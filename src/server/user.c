/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function definitions for basic user operations.
 * @details Basic user operations include user registration, verification, and secret manipulation.
 */

#include <string.h>

#include "user.h"
#include "ipc.h"
#include "database.h"

#include "../share/utils.h"

/**
 * @brief Check if a user exists in the database `database`.
 * @details Iterates through the whole list.
 * @param database The database to look for the user in.
 * @param username The username to look for in the database.
 * @return `true` if the user was found, `false` otherwise.
 */
static bool user_exists(list_t *database, char *username)
{
	for (element_t *curr = database->head; curr != NULL; curr = curr->next) {
		entry_t *e = (entry_t *) curr->data;
		if (strncmp(e->username, username, MAX_USERNAME_LEN + 1) == 0)
			return true;
	}

	return false;
}

bool user_register(list_t *database, char *username, char *password)
{
	if (user_exists(database, username))
		return false;

	str_strip(username);
	str_strip(password);

	if (!is_valid_field(username, false) ||
		!is_valid_field(password, false))
		return false;

	entry_t e;
	memset(&e, 0, sizeof(e));
	strncpy(e.username, username, MAX_USERNAME_LEN + 1);
	strncpy(e.password, password, MAX_PASSWORD_LEN + 1);

	bool success = list_add(database, &e, sizeof(e));
	return success;
}

bool user_verify_credentials(list_t *database, char *username, char *password)
{
	for (element_t *curr = database->head; curr != NULL; curr = curr->next) {
		entry_t *e = (entry_t *) curr->data;
		if (strncmp(e->username, username, MAX_USERNAME_LEN) == 0 &&
			strncmp(e->password, password, MAX_PASSWORD_LEN) == 0)
			return true;
	}

	return false;
}

bool user_login(list_t *clients, char *username, char *session_id)
{
	client_t c;
	memset(&c, 0, sizeof(c));
	strncpy(c.session_id, session_id, SESSION_ID_SIZE + 1);
	strncpy(c.username, username, MAX_USERNAME_LEN + 1);

	return list_add(clients, &c, sizeof(client_t));
}

bool user_logout(list_t *clients, char *username, char *session_id)
{
	for (element_t *curr = clients->head; curr != NULL; curr = curr->next) {
		client_t *e = (client_t *) curr->data;

		if (strncmp(e->username, username, MAX_USERNAME_LEN) == 0 &&
			strncmp(e->session_id, session_id, SESSION_ID_SIZE) == 0) {
			return list_remove(clients, e);
		}
	}

	return false;
}

char *user_secret_read(list_t *database, char *username)
{
	for (element_t *curr = database->head; curr != NULL; curr = curr->next) {
		entry_t *e = (entry_t *) curr->data;
		if (strncmp(e->username, username, MAX_USERNAME_LEN) == 0) {
			return e->secret;
		}
	}

	return NULL;
}

bool user_secret_write(list_t *database, char *username, char *secret)
{
	str_strip(secret);

	if (!is_valid_field(secret, true))
		return false;

	for (element_t *curr = database->head; curr != NULL; curr = curr->next) {
		entry_t *e = (entry_t *) curr->data;
		if (strncmp(e->username, username, MAX_USERNAME_LEN) == 0) {
			strncpy(e->secret, secret, MAX_SECRET_LEN + 1);
			return true;
		}
	}

	return false;
}
