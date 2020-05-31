/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function definitions for basic user management.
 * @details Basic user management includes functions to register, login, logout.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>

#include "user.h"
#include "utils.h"

#include "../share/utils.h"
#include "../share/protocol.h"

/**
 * @brief The memory shared between the server and the clients.
 * @details This shared memory is assumed to be available when functions of this module are used.
 */
extern void *shmem;

/**
 * @brief The first server semaphore.
 * @details This semaphore is assumed to be available when functions of this module are used.
 */
extern sem_t *sem1;

/**
 * @brief The second server semaphore.
 * @details This semaphore is assumed to be available when functions of this module are used.
 */
extern sem_t *sem2;

/**
 * @brief The client semaphore.
 * @details This semaphore is assumed to be available when functions of this module are used.
 */
extern sem_t *sem3;

/**
 * @brief Wait for the server to process the packet in the shared memory.
 * @details The server is given access to the shared memory. This function makes use of two global variables `sem1` and `sem2`.
 */
static void send_packet(void)
{
	// grant write access for the server
	sem_post(sem1);

	// request write access to the shared memory
	sem_wait_checked(sem2);
}

int register_user(options_t *options)
{
	int val;

	// enter server request queue
	sem_wait_checked(sem3);

	// request write access to the shared memory
	sem_wait_checked(sem2);

	struct packet_registration *p = shmem;
	p->type = REGISTRATION;
	strncpy(p->username, options->username, MAX_USERNAME_LEN + 1);
	strncpy(p->password, options->password, MAX_PASSWORD_LEN + 1);

	send_packet();

	if (p->rstatus == ERROR)
		val = 1;
	else
		val = 0;

	// grant write access for the server
	sem_post(sem1);

	// leave server request queue
	sem_post_checked(sem3);

	return val;
}

int login_user(options_t *options, char *session_id)
{
	int val;

	// enter server request queue
	sem_wait_checked(sem3);

	// request write access to the shared memory
	sem_wait_checked(sem2);

	struct packet_login *p = shmem;
	p->type = LOGIN;
	strncpy(p->username, options->username, MAX_USERNAME_LEN + 1);
	strncpy(p->password, options->password, MAX_PASSWORD_LEN + 1);

	send_packet();

	p->session_id[SESSION_ID_SIZE] = '\0';
	size_t len = strlen(p->session_id);
	strncpy(session_id, p->session_id, SESSION_ID_SIZE + 1);

	if (len == SESSION_ID_SIZE)
		val = 0;
	else
		val = 1;

	// grant write access for the server
	sem_post(sem1);

	// leave server request queue
	sem_post_checked(sem3);

	return val;
}

int logout_user(options_t *options, char *session_id)
{
	int val;

	// enter server request queue
	sem_wait_checked(sem3);

	// request write access to the shared memory
	sem_wait_checked(sem2);

	struct packet_logout *p = shmem;
	p->type = LOGOUT;
	strncpy(p->session_id, session_id, SESSION_ID_SIZE + 1);
	strncpy(p->username, options->username, MAX_USERNAME_LEN + 1);

	send_packet();

	if (p->rstatus == ERROR)
		val = 1;
	else
		val = 0;

	// grant write access for the server
	sem_post(sem1);

	// leave server request queue
	sem_post_checked(sem3);

	return val;
}

int write_secret(options_t *options, char *session_id, char *secret)
{
	int val;

	// enter server request queue
	sem_wait_checked(sem3);

	// request write access to the shared memory
	sem_wait_checked(sem2);

	struct packet_secret_write *p = shmem;
	p->type = SECRET_WRITE;
	strncpy(p->session_id, session_id, SESSION_ID_SIZE + 1);
	strncpy(p->username, options->username, MAX_USERNAME_LEN + 1);
	strncpy(p->secret, secret, MAX_SECRET_LEN + 1);

	send_packet();

	if (p->rstatus == ERROR)
		val = 1;
	else
		val = 0;

	// grant write access for the server
	sem_post(sem1);

	// leave server request queue
	sem_post_checked(sem3);

	return val;
}

int read_secret(options_t *options, char *session_id, char *secret)
{
	int val;

	// enter server request queue
	sem_wait_checked(sem3);

	// request write access to the shared memory
	sem_wait_checked(sem2);

	struct packet_secret_read *p = shmem;
	p->type = SECRET_READ;
	strncpy(p->session_id, session_id, SESSION_ID_SIZE + 1);
	strncpy(p->username, options->username, MAX_USERNAME_LEN + 1);

	send_packet();

	p->secret[MAX_SECRET_LEN] = '\0';
	strncpy(secret, p->secret, MAX_SECRET_LEN + 1);

	if (p->rstatus == ERROR)
		val = 1;
	else
		val = 0;

	// grant write access for the server
	sem_post(sem1);

	// leave server request queue
	sem_post_checked(sem3);

	return val;
}
