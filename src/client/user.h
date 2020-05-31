/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function declarations for basic user management.
 * @details Basic user management includes functions to register, login, logout.
 */

#ifndef __USER_H__
#define __USER_H__

#include "options.h"

/**
 * @brief Register a new user on the server.
 * @details This method requires the precense of the global variables `sem1`, `sem2`, `sem3` and `shmem`.
 * @param options The program configuration.
 * @return `0` on success, `1` otherwise.
 */
int register_user(options_t *options);

/**
 * @brief Login the user on the server side.
 * @details This method requires the precense of the global variables `sem1`, `sem2`, `sem3` and `shmem`.
 * @param options The program configuration.
 * @param session_id The session id retrieved on user login.
 * @return `0` on success, `1` otherwise.
 */
int login_user(options_t *options, char *session_id);

/**
 * @brief Logout the user on the server side.
 * @details This method requires the precense of the global variables `sem1`, `sem2`, `sem3` and `shmem`.
 * @param options The program configuration.
 * @param session_id The session id retrieved on user login.
 * @return `0` on success, `1` otherwise.
 */
int logout_user(options_t *options, char *session_id);

/**
 * @brief Write a secret to the database on the server.
 * @details This method requires the precense of the global variables `sem1`, `sem2`, `sem3` and `shmem`.
 * @param options The program configuration.
 * @param session_id The session id retrieved on user login.
 * @param secret The secret to write to the database.
 * @return `0` on success, `1` otherwise.
 */
int write_secret(options_t *options, char *session_id, char *secret);

/**
 * @brief Read the stored secret in the database on the server.
 * @details This method requires the precense of the global variables `sem1`, `sem2`, `sem3` and `shmem`.
 * @param options The program configuration.
 * @param session_id The session id retrieved on user login.
 * @param secret The buffer to read the secret into.
 * @return `0` on success, `1` otherwise.
 */
int read_secret(options_t *options, char *session_id, char *secret);

#endif
