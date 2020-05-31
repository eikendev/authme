/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains the core functionality of the client.
 * @details Core functionality includes functions to get new instructions and resource setup.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <semaphore.h>

#include "options.h"
#include "user.h"
#include "instruction.h"

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
volatile sig_atomic_t running = true;

/**
 * @brief The memory shared between the server and the clients.
 */
void *shmem = NULL;

/**
 * @brief The file descriptor used to open the shared memory.
 */
static int memfd = -1;

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
sem_t *sem3 = NULL;

/**
 * @brief Signal handler for the client.
 * @details The `running` variable is set to `false` on `SIGTERM` or `SIGINT` signal interruption.
 * @param signum The signal id.
 */
static void handler(int signum)
{
	running = false;
}

/**
 * @brief The client cleanup function.
 * @details This function handles resource cleanup on program termination.
 */
static void cleanup(void)
{
	int errind;

	// cleanup shared memory
	if (memfd >= 0) {
		errind = close_shared_memory(SHM_NAME, SHM_LEN, memfd, false);
		if (errind != 0)
			print_error("failed closing shared memory");
	}

	// cleanup semaphores
	sem_cleanup(sem1, NULL);
	sem_cleanup(sem2, NULL);
	sem_cleanup(sem3, NULL);
}

/**
 * @brief Menu printing function.
 * @details This function is used to print the menu to request the next instruction from the user.
 */
static void print_menu(void)
{
	printf("Commands:\n");
	printf("  1) write secret\n");
	printf("  2) read secret\n");
	printf("  3) logout\n");
	printf("Please select a command (1-3): ");
	fflush(stdout);
}

/**
 * @brief Instruction fetching function.
 * @details This function is used to request an instruction from the user.
 * @return The next instruction or `-1` on error.
 */
static int get_next_instruction(void)
{
	int instruction = 0;

	while (instruction < 1 || instruction > 3) {
		// signal might have arrived
		if (!running)
			return -1;

		print_menu();
		char tmp_instr;
		tmp_instr = getchar();

		if (tmp_instr == EOF)
			continue;

		instruction = tmp_instr - '0';
		clear_buffer(stdin);
	}

	return instruction;
}

/**
 * @brief The main loop of the client program.
 * @details This core functionality of the client is bound to this loop. Two steps are executed continuously: instruction fetching and instruction handling.
 * @param options The program configuration.
 * @param session_id The session_id retrieved while logging in.
 */
static void run_main_loop(options_t *options, char *session_id)
{
	while (running) {
		int instruction = get_next_instruction();

		if (instruction == -1)
			break;

		handle_instruction(instruction, options, session_id);
	}
}

/**
 * @brief Enty point of the client.
 * @details This function is called upon program start. First, resources are set up and `argv` is parsed. Then the client waits for instructions from the user, which are redirected to the server according to protocol specifications.
 * @param argc Cardinality of `argv`.
 * @param argv Program argument vector.
 * @return `EXIT_FAILURE` on error, `EXIT_SUCCESS` otherwise.
 */
int main(int argc, char *argv[])
{
	int errind;

	progname = argv[0];

	struct sigaction act;
	memset(&act, 0, sizeof(act));
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

	options_t options;
	parse_arguments(argc, argv, &options);

	sem1 = sem_open(SEM_SERVER1, 0);
	if (sem1 == SEM_FAILED)
		print_error_exit("failed opening semaphore");

	sem2 = sem_open(SEM_SERVER2, 0);
	if (sem2 == SEM_FAILED)
		print_error_exit("failed opening semaphore");

	sem3 = sem_open(SEM_CLIENT1, 0);
	if (sem3 == SEM_FAILED)
		print_error_exit("failed opening semaphore");

	memfd = create_shared_memory(SHM_NAME, SHM_LEN, false);
	if (memfd < 0)
		print_error_exit("failed creating shared memory");

	if (options.mode == CMD_REGISTER) {
		errind = register_user(&options);

		if (errind) {
			fprintf(stderr, "Registration failed.\n");
			exit(EXIT_FAILURE);
		} else {
			fprintf(stderr, "Registration successful.\n");
		}
	} else if (options.mode == CMD_LOGIN) {
		char session_id[SESSION_ID_SIZE + 1];
		errind = login_user(&options, session_id);

		if (errind) {
			fprintf(stderr, "Login failed.\n");
			exit(EXIT_FAILURE);
		} else {
			fprintf(stderr, "Login successful.\n");
			run_main_loop(&options, session_id);
		}
	} else {
		assert(false);
	}

	return EXIT_SUCCESS;
}
