/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains the core functionality of the server.
 * @details Core functionality includes functions to wait for clients to connect and to shut down the server.
 */

#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

#include "options.h"
#include "utils.h"
#include "list.h"
#include "ipc.h"
#include "database.h"

#include "../share/utils.h"
#include "../share/shmem.h"
#include "../share/protocol.h"

/**
 * @brief Program name.
 * @details This variable must be set on program start.
 */
char *progname;

/**
 * @brief Indicator for the program to shut down.
 */
static volatile sig_atomic_t running = true;

/**
 * @brief The memory shared between the server and the clients.
 */
void *shmem = NULL;

/**
 * @brief The file descriptor used to open the shared memory.
 */
static int memfd = -1;

/**
 * @brief Program configuration.
 * @details This struct keeps the configuration retrived by parsing program arguments at program start.
 */
static options_t options;

/**
 * @brief The first server semaphore.
 * @details This semaphore is used on the server side to wait for client processing of the shared memory.
 */
sem_t *sem1 = NULL;

/**
 * @brief The second server semaphore.
 * @details This semaphore is used on the client side to wait for server processing of the shared memory.
 */
sem_t *sem2 = NULL;

/**
 * @brief The client semaphore.
 * @details This semaphore is used to make sure only one client can communicate with the server at a time.
 */
static sem_t *sem3 = NULL;

/**
 * @brief The client list.
 * @details This list is used to keep track of who is currently logged in.
 */
list_t *clients = NULL;

/**
 * @brief The database list.
 * @details This list is used to keep an in-memory copy of the database.
 */
list_t *database = NULL;

/**
 * @brief Signal handler for the server.
 * @details The `running` variable is set to `false` on `SIGTERM` or `SIGINT` signal interruption.
 * @param signum The signal id.
 */
static void handler(int signum)
{
	running = false;
}

/**
 * @brief The server cleanup function.
 * @details This function handles resource cleanup as well as client wakeup.
 */
static void cleanup(void)
{
	int errind;

	// set server flag to offline
	set_status_offline(shmem);

	// wake up waiting clients
	if (sem3 != NULL)
		sem_settle(sem3);

	// cleanup shared memory
	if (memfd >= 0) {
		errind = close_shared_memory(SHM_NAME, SHM_LEN, memfd, true);
		if (errind != 0)
			print_error("failed closing shared memory");
	}

	// cleanup semaphores
	sem_cleanup(sem1, SEM_SERVER1);
	sem_cleanup(sem2, SEM_SERVER2);
	sem_cleanup(sem3, SEM_CLIENT1);

	// save database to file
	if (options.database_path != NULL) {
		errind = save_database(options.database_path, database);
		if (errind != 0)
			print_error_plain("could not save the database");
	}

	// cleanup lists
	list_destroy(clients);
	list_destroy(database);
}

/**
 * @brief The main loop of the server program.
 * @details This core functionality of the server is bound to this loop. Two steps are executed continuously: packet retrieval and packet handling.
 */
static void run_main_loop(void)
{
	int errind;

	while (running) {
		// wait for a client to grant write access
		errind = sem_wait_exit(sem1, true);
		if (errind != 0)
			break;

		handle_packet(shmem);

		// prevent posting next client if not longer running
		if (!running)
			break;

		errind = sem_post(sem2);
		if (errind != 0)
			print_error_exit("failed posting semaphore");
	}
}

/**
 * @brief Enty point of the server.
 * @details This function is called upon program start. First, resources are set up and `argv` is parsed. Then, the database is read whereupon the server waits for a clients to connect and handles incoming requests. On server termination, resources are cleaned up and the database is saved.
 * @param argc Cardinality of `argv`.
 * @param argv Program argument vector.
 * @return `EXIT_FAILURE` on error, `EXIT_SUCCESS` otherwise.
 */
int main(int argc, char *argv[])
{
	int errind;

	progname = argv[0];
	srand((unsigned) time(NULL));

	struct sigaction act;
	memset(&act, 0, sizeof (act));
	act.sa_handler = handler;

	errind = sigaction(SIGINT, &act, NULL);
	if (errind == -1)
		print_error_exit("failed registering signal handler");

	errind = sigaction(SIGTERM, &act, NULL);
	if (errind == -1)
		print_error_exit("failed registering signal handler");

	errind = atexit(cleanup);
	if (errind != 0)
		print_error_exit("failed registering cleanup function");

	parse_arguments(argc, argv, &options);

	database = list_initialize();
	if (database == NULL)
		print_error_plain_exit("failed initializing database list");

	clients = list_initialize();
	if (clients == NULL)
		print_error_plain_exit("failed initializing clients list");

	if (options.database_path != NULL) {
		errind = read_database(&options.database_path, database);
		if (errind != 0)
			print_error_plain_exit("failed reading database");
	}

	sem1 = sem_open(SEM_SERVER1, O_CREAT | O_EXCL, 0660, 0);
	if (sem1 == SEM_FAILED)
		print_error_exit("failed opening semaphore");

	sem2 = sem_open(SEM_SERVER2, O_CREAT | O_EXCL, 0660, 1);
	if (sem2 == SEM_FAILED)
		print_error_exit("failed opening semaphore");

	sem3 = sem_open(SEM_CLIENT1, O_CREAT | O_EXCL, 0660, 1);
	if (sem3 == SEM_FAILED)
		print_error_exit("failed opening semaphore");

	memfd = create_shared_memory(SHM_NAME, SHM_LEN, true);
	if (memfd < 0)
		print_error_exit("failed creating shared memory");

	// set server flag to online
	set_status_online(shmem);

	run_main_loop();

	return EXIT_SUCCESS;
}
