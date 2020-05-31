/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function declarations to handle user instructions.
 * @details The user can instruct to write a new secret or to read the stored one.
 */

#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "options.h"

/**
 * @brief Handle a user instruction.
 * @details Determines and delegates the further proceedings to a instruction specific function. This method requires the precense of the global variables `sem1`, `sem2`, `sem3` and `shmem`.
 * @param instruction The instruction the user wants to execute.
 * @param options The programs configuration.
 * @param session_id The session id retrieved when logging in on the server.
 */
void handle_instruction(int instruction, options_t *options, char *session_id);

#endif
