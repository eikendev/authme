/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function definitions for interprocess communication.
 * @details The functions include the ability to handle packets from clients.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>

#include "ipc.h"
#include "user.h"
#include "list.h"

#include "../share/protocol.h"
#include "../share/utils.h"

/**
 * @brief The memory shared between the server and the clients.
 * @details This variable is required for the use of this module.
 */
extern void *shmem;

/**
 * @brief The first server semaphore.
 * @details This variable is required for the use of this module.
 */
extern sem_t *sem1;

/**
 * @brief The second server semaphore.
 * @details This variable is required for the use of this module.
 */
extern sem_t *sem2;

/**
 * @brief The client list.
 * @details This variable is required for the use of this module.
 */
extern list_t *clients;

/**
 * @brief The database list.
 * @details This variable is required for the use of this module.
 */
extern list_t *database;

/**
 * @brief Generate a random session id.
 * @param buffer The buffer to write the session id to.
 */
static void generate_session_id(char buffer[SESSION_ID_SIZE])
{
	static const char alphanum[] =
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789";

	for (int i = 0; i < SESSION_ID_SIZE; i++) {
		int index = randd() * (sizeof(alphanum) - 1);
		char next = alphanum[index];
		buffer[i] = next;
	}
}

/**
 * @brief Check if user `username` is actually logged in.
 * @details This function makes use of the global variables `shmem`, `sem1`, `sem2`, `clients` and `database`.
 * @param session_id The session id to validate.
 * @param username The username to validate.
 * @return `true` if the session is valid, `false` otherwise.
 */
static bool is_valid_session(char *session_id, char *username)
{
	for (element_t *curr = clients->head; curr != NULL; curr = curr->next) {
		client_t *c = (client_t *) curr->data;
		if (strncmp(c->session_id, session_id, SESSION_ID_SIZE) == 0 &&
			strncmp(c->username, username, MAX_USERNAME_LEN) == 0)
			return true;
	}

	return false;
}

/**
 * @brief Process a registration packet.
 * @details This function makes use of the global variables `shmem`, `sem1`, `sem2`, `clients` and `database`.
 * @param packet The packet to handle.
 */
static void process_registration(void *packet)
{
	struct packet_registration *p = (struct packet_registration *) packet;
	p->username[MAX_USERNAME_LEN] = '\0';
	p->password[MAX_PASSWORD_LEN] = '\0';

	if (user_register(database, p->username, p->password))
		p->rstatus = SUCCESS;
	else
		p->rstatus = ERROR;
}

/**
 * @brief Process a login packet.
 * @details This function makes use of the global variables `shmem`, `sem1`, `sem2`, `clients` and `database`.
 * @param packet The packet to handle.
 */
static void process_login(void *packet)
{
	struct packet_login *p = (struct packet_login *) packet;
	p->username[MAX_USERNAME_LEN] = '\0';
	p->password[MAX_PASSWORD_LEN] = '\0';

	if (user_verify_credentials(database, p->username, p->password)) {
		generate_session_id(p->session_id);
		p->session_id[SESSION_ID_SIZE] = '\0';

		user_login(clients, p->username, p->session_id);
	} else {
		memset(p->session_id, '\0', SESSION_ID_SIZE + 1);
	}
}

/**
 * @brief Process a logout packet.
 * @details This function makes use of the global variables `shmem`, `sem1`, `sem2`, `clients` and `database`.
 * @param packet The packet to handle.
 */
static void process_logout(void *packet)
{
	struct packet_logout *p = (struct packet_logout *) packet;
	p->session_id[SESSION_ID_SIZE] = '\0';
	p->username[MAX_USERNAME_LEN] = '\0';

	if (is_valid_session(p->session_id, p->username)) {
		bool success = user_logout(clients, p->username, p->session_id);

		if (success)
			p->rstatus = SUCCESS;
		else
			p->rstatus = ERROR;
	} else {
		p->rstatus = ERROR;
	}
}

/**
 * @brief Process a secret_write packet.
 * @details This packet is sent if the user wishes to change their secret. This function makes use of the global variables `shmem`, `sem1`, `sem2`, `clients` and `database`.
 * @param packet The packet to handle.
 */
static void process_secret_write(void *packet)
{
	struct packet_secret_write *p = (struct packet_secret_write *) packet;
	p->session_id[SESSION_ID_SIZE] = '\0';
	p->username[MAX_USERNAME_LEN] = '\0';
	p->secret[MAX_SECRET_LEN] = '\0';

	if (is_valid_session(p->session_id, p->username)) {
		bool success = user_secret_write(database, p->username, p->secret);

		if (success)
			p->rstatus = SUCCESS;
		else
			p->rstatus = ERROR;
	} else {
		p->rstatus = ERROR;
	}
}

/**
 * @brief Process a secret_read packet.
 * @details This packet is sent if the user wishes to read their secret. This function makes use of the global variables `shmem`, `sem1`, `sem2`, `clients` and `database`.
 * @param packet The packet to handle.
 */
static void process_secret_read(void *packet)
{
	struct packet_secret_read *p = (struct packet_secret_read *) packet;
	p->session_id[SESSION_ID_SIZE] = '\0';
	p->username[MAX_USERNAME_LEN] = '\0';

	if (is_valid_session(p->session_id, p->username)) {
		char *secret = user_secret_read(database, p->username);

		if (secret != NULL)
			strncpy(p->secret, secret, MAX_SECRET_LEN);
		else
			memset(p->secret, '\0', MAX_SECRET_LEN);
	} else {
		memset(p->secret, '\0', MAX_SECRET_LEN);
	}
}

void handle_packet(void *packet)
{
	struct packet_generic *pg = packet;

	switch (pg->type) {
	case REGISTRATION:
		process_registration(shmem);
		break;
	case LOGIN:
		process_login(shmem);
		break;
	case LOGOUT:
		process_logout(shmem);
		break;
	case SECRET_WRITE:
		process_secret_write(shmem);
		break;
	case SECRET_READ:
		process_secret_read(shmem);
		break;
	default:
		assert(false);
	}

	// notify client about data arrival
	sem_post(sem2);

	// wait for the client to process the data
	sem_wait_exit(sem1, false);

	// make sure next client cannot read other secrets
	memset(shmem, 0, SHM_LEN);
	pg->status = ONLINE;
}
