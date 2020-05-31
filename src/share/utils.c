/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains the shared utility function definitons.
 * @details Functions in this module are not program specific.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "utils.h"

/**
 * @brief Program name.
 * @details This variable must be set on program start.
 */
extern char *progname;

void clear_buffer(FILE *buffer)
{
	int c;
	while (((c = getc(buffer)) != '\n') && c != EOF);
}

double randd(void)
{
	return rand() / ((double) RAND_MAX + 1);
}

void sem_cleanup(sem_t *sem, char *sem_path)
{
	int errind;

	if (sem != NULL && sem != SEM_FAILED) {
		errind = sem_close(sem);
		if (errind == -1)
			print_error("failed closing semaphore");

		if (sem_path != NULL) {
			errind = sem_unlink(sem_path);
			if (errind == -1)
				print_error("failed unlinking semaphore");
		}
	}
}

int sem_wait_exit(sem_t *sem, bool error_only)
{
	int errind = sem_wait(sem);

	if (errind != 0) {
		switch (errno) {
		case EINTR:
			if (!error_only)
				exit(EXIT_SUCCESS);

			break;
		default:
			print_error_exit("failed waiting for semaphore");
		}

		return 1;
	}

	return 0;
}

void sem_settle(sem_t *sem)
{
	int errind, sval;

	errind = sem_getvalue(sem, &sval);

	if (errind == -1)
		print_error_exit("failed retrieving semaphore value");

	for (; sval < 1; sval++) {
		errind = sem_post(sem);
	}
}

void str_strip(char *str)
{
	size_t len = strlen(str);
	bool has_content = false;
	uint16_t current = 0;

	for (size_t i = 0; i < len; i++) {
		if (has_content || !isspace(str[i])) {
			str[current++] = str[i];
			has_content = true;
		}
	}

	str[current] = '\0';

	for (int i = current - 1; i >= 0; i--) {
		if (!isspace(str[i])) {
			str[i + 1] = '\0';
			break;
		}
	}
}

bool is_valid_field(char *str, bool allow_empty)
{
	char *semptr;
	size_t len = strlen(str);

	if (len == 0)
		return allow_empty;

	semptr = strchr(str, '\n');
	if (semptr != NULL)
		return false;

	semptr = strchr(str, ';');
	if (semptr != NULL)
		return false;

	return true;
}

void print_error_plain(char *msg)
{
	fprintf(stderr, "[%s]: ERROR: %s\n", progname, msg);
}

void print_error(char *msg)
{
	fprintf(stderr, "[%s]: ERROR: %s: %s\n", progname, msg, strerror(errno));
}

void print_error_plain_exit(char *msg)
{
	print_error_plain(msg);
	exit(EXIT_FAILURE);
}

void print_error_exit(char *msg)
{
	print_error(msg);
	exit(EXIT_FAILURE);
}
