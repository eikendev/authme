/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function definitions for basic database operations.
 * @details Basic database operations include database reading and database writing.
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "database.h"

#include "../share/utils.h"

int read_database(char **path, list_t *database)
{
	int errind = 0;

	if (*path == NULL || database == NULL)
		return 1;

	FILE *fp = fopen(*path, "r");
	if (fp == NULL)
		print_error_exit("failed opening file");

	char *line = NULL;
	size_t len_alloc = 0;
	ssize_t len_line;
	while ((len_line = getline(&line, &len_alloc, fp)) != -1) {
		entry_t e;
		memset(&e, 0, sizeof(e));

		sscanf(line, "%[^\n;];%[^\n;];%[^\n;]\n", e.username, e.password, e.secret);
		free(line);

		if (!is_valid_field(e.username, false) ||
			!is_valid_field(e.password, false)) {
			errind = 2;
			*path = NULL;
			break;
		}

		list_add(database, (obj_t) &e, sizeof(entry_t));
	}

	fclose(fp);

	return errind;
}

int save_database(char *path, list_t *database)
{
	if (path == NULL || database == NULL)
		return 1;

	FILE *fp = fopen(path, "w");
	if (fp == NULL) {
		print_error("failed opening file");
		return 2;
	}

	for (element_t *curr = database->head; curr != NULL; curr = curr->next) {
		entry_t *e = (entry_t *) curr->data;
		fprintf(fp, "%s;%s;%s\n", e->username, e->password, e->secret);
	}

	fclose(fp);

	return 0;
}
