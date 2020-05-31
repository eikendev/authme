/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function declarations for basic user operations.
 * @details Basic user operations include user registration, verification, and secret manipulation.
 */

#ifndef __USER_H__
#define __USER_H__

#include "list.h"

/**
 * @brief Register the user `username` in the database.
 * @param database The database to consider for this operation.
 * @param username The username to consider for this operation.
 * @param password The password to assign the username to.
 * @return `true` on success, `false` otherwise.
 */
bool user_register(list_t *database, char *username, char *password);

/**
 * @brief Verify if `password` is the password for the user `username`.
 * @param database The database to consider for this operation.
 * @param username The username to consider for this operation.
 * @param password The password to use for the verification.
 * @return `true` on success, `false` otherwise.
 */
bool user_verify_credentials(list_t *database, char *username, char *password);

/**
 * @brief Add the user to the clients list.
 * @param clients The clients list to consider for this operation.
 * @param username The username to consider for this operation.
 * @param session_id Session id associated with this user.
 * @return `true` on success, `false` otherwise.
 */
bool user_login(list_t *clients, char *username, char *session_id);

/**
 * @brief Remove the user from the clients list.
 * @param database The database to consider for this operation.
 * @param username The username to consider for this operation.
 * @param session_id Session id associated with this user.
 * @return `true` on success, `false` otherwise.
 */
bool user_logout(list_t *database, char *username, char *session_id);

/**
 * @brief Read the secret of the user `username` from the database.
 * @param database The database to consider for this operation.
 * @param username The username to consider for this operation.
 * @return A pointer to the secret string.
 */
char *user_secret_read(list_t *database, char *username);

/**
 * @brief Write a new secret for the user `username` to the database.
 * @param database The database to consider for this operation.
 * @param username The username to consider for this operation.
 * @param secret The secret to write into the database.
 * @return `true` on success, `false` otherwise.
 */
bool user_secret_write(list_t *database, char *username, char *secret);

#endif
