/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function definitions to handle user instructions.
 * @details The user can instruct to write a new secret or to read the stored one.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <signal.h>

#include "user.h"

#include "../share/utils.h"
#include "../share/protocol.h"

/**
 * @brief Indicator for the program to shut down.
 * @details This global variable is assumed to be available when using this module.
 */
extern volatile sig_atomic_t running;

/**
 * @brief Write a new secret to the database.
 * @details This function prompts the user to input the new secret and notifies the server about the update.
 * @param options The programs configuration.
 * @param session_id The session id retrieved when logging in on the server.
 */
static void handle_secret_write(options_t *options, char *session_id)
{
	printf("New secret: ");
	fflush(stdout);

	char *line = NULL;
	size_t len_alloc = 0;
	ssize_t len_line = getline(&line, &len_alloc, stdin);

	if (len_line == -1) {
		fprintf(stderr, "Could not read your input.\n");
	} else if (len_line > MAX_SECRET_LEN) {
		fprintf(stderr, "Your secret is too long.\n");
	} else {
		line[MAX_SECRET_LEN] = '\0';
		int errind = write_secret(options, session_id, line);

		if (errind != 0)
			fprintf(stderr, "Could not write your new secret.\n");
	}

	if (line != NULL)
		free(line);
}

/**
 * @brief Read the secret stored in the database.
 * @details This function requests the secret and prints it to `stdout`.
 * @param options The programs configuration.
 * @param session_id The session id retrieved when logging in on the server.
 */
static void handle_secret_read(options_t *options, char *session_id)
{
	char secret[MAX_SECRET_LEN + 1];
	int errind = read_secret(options, session_id, secret);

	if (errind == 0)
		printf("Your secret: %s\n", secret);
	else
		fprintf(stderr, "Could not read the secret.\n");
}

/**
 * @brief Logout of the server.
 * @details This function notifies the server about the user logout.
 * @param options The programs configuration.
 * @param session_id The session id retrieved when logging in on the server.
 */
static void handle_logout(options_t *options, char *session_id)
{
	int errind = logout_user(options, session_id);

	if (errind != 0)
		fprintf(stderr, "Could not logout correctly.\n");

	running = false;
}

void handle_instruction(int instruction, options_t *options, char *session_id)
{
	switch (instruction) {
	case 1:
		handle_secret_write(options, session_id);
		break;
	case 2:
		handle_secret_read(options, session_id);
		break;
	case 3:
		handle_logout(options, session_id);
		break;
	default:
		assert(false);
	}
}
