/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains the shared utility function declarations.
 * @details Functions in this module are not program specific.
 */

#ifndef __UTILS_H_SHARE__
#define __UTILS_H_SHARE__

#include <stdio.h>
#include <stdbool.h>

#include <semaphore.h>

/**
 * @brief Clear a buffer until `EOF` or newline is reached.
 * @details The function might be slow due to internal usage of `getc()`.
 * @param buffer The buffer to clear.
 */
void clear_buffer(FILE *buffer);

/**
 * @brief Generate a random number.
 * @details The number will be greater or equal to `0`, but strictly less than `1`.
 * @return The random number generated.
 */
double randd(void);

/**
 * @brief Clean up a semaphore.
 * @details If `sem_path` is not `NULL`, then the semaphore will be unlinked.
 * @param sem Semaphore to clean up.
 * @param sem_path Filename of the semaphore.
 */
void sem_cleanup(sem_t *sem, char *sem_path);

/**
 * @brief Wait for semaphore `sem` and exit, if an error occures.
 * @details When exiting, an error message will be printed to `stderr`. This function makes use of the global variable `progname` in order to print the program name along with the error message.
 * @param sem Semaphore to wait for.
 * @param error_only If true, will not exit when interrupted by a signal. Otherwise, a signal interruption is treated as error.
 * @return `0` on success, `1` on error.
 */
int sem_wait_exit(sem_t *sem, bool error_only);

/**
 * @brief Settle a semaphore.
 * @details This function posts as many times as needed in order for the semaphore value to become positive.
 * @param sem The semaphore to settle.
 */
void sem_settle(sem_t *sem);

/**
 * @brief Remove the outer whitespace of a string.
 * @details The string will be modified in place.
 * @param str String to strip.
 */
void str_strip(char *str);

/**
 * @brief Check if the string specified meets certain requirements.
 * @details The string will be checked for emptiness and occurences of disallowed characters.
 * @param str String to validate.
 * @param allow_empty Specifies whether empty strings are allowed or not.
 * @return `true` for a valid string, `false` for an invalid string.
 */
bool is_valid_field(char *str, bool allow_empty);

/**
 * @brief Print an error message.
 * @details This functions uses the global variable `progname` in order to print the program name along with the message.
 * @param msg Message to print to `stderr`.
 */
void print_error_plain(char *msg);

/**
 * @brief Print an error message.
 * @details This functions uses the global variable `progname` in order to print the program name along with the message. `errno` will be used to describe the reason for the error.
 * @param msg Message to print to `stderr`.
 */
void print_error(char *msg);

/**
 * @brief Print an error message and exit.
 * @details This functions uses the global variable `progname` in order to print the program name along with the message.
 * @param msg Message to print to `stderr`.
 */
void print_error_plain_exit(char *msg);

/**
 * @brief Print an error message and exit.
 * @details This functions uses the global variable `progname` in order to print the program name along with the message. `errno` will be used to describe the reason for the error.
 * @param msg Message to print to `stderr`.
 */
void print_error_exit(char *msg);

#endif
