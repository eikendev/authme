/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function declarations for basic database operations.
 * @details Basic database operations include database reading and database writing.
 */

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <stdio.h>

#include "list.h"

#include "../share/protocol.h"

/**
 * @brief Struct for a database entry.
 */
typedef struct {
	char username[MAX_USERNAME_LEN + 1]; ///< Username field of the entry.
	char password[MAX_PASSWORD_LEN + 1]; ///< Password field of the entry.
	char secret[MAX_SECRET_LEN + 1]; ///< Secret field of the entry.
} entry_t;

/**
 * @brief Read a database from a file.
 * @details Lines are evaluated separately for database entries.
 * @param path Path to the file to read from.
 * @param database List to store the database in.
 * @return `0` on success, positive integer on failure.
 */
int read_database(char **path, list_t *database);

/**
 * @brief Write a database to a file.
 * @details Lines are written iteratively for each entry.
 * @param path Path to the file to write to.
 * @param database List to store the database in.
 * @return `0` on success, positive integer on failure.
 */
int save_database(char *path, list_t *database);

#endif
