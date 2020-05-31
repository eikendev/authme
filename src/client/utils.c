/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains the client utility function definitions.
 * @details Functions in this module are not program specific.
 */

#include "utils.h"

#include "../share/utils.h"
#include "../share/protocol.h"

/**
 * @brief The memory shared between the server and the clients.
 * @details This variable is assumed to be available when functions of this module are used.
 */
extern void *shmem;

void sem_wait_checked(sem_t *sem)
{
	int status;

	struct packet_generic *p = shmem;
	status = p->status;

	if (status == OFFLINE)
		print_error_plain_exit("server is not available");
	else if (status == ONLINE)
		sem_wait(sem);
}

void sem_post_checked(sem_t *sem)
{
	int status;

	struct packet_generic *p = shmem;
	status = p->status;

	if (status == OFFLINE)
		print_error_plain_exit("server is not available");
	else if (status == ONLINE)
		sem_post(sem);
}
