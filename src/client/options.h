/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module declares functions to handle the clients's argument parsing.
 * @details Argument parsing is done via `getopt()`.
 */

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

/**
 * @brief Program modes.
 * @details This enum is used to specify whether to login or to register.
 */
typedef enum {
	CMD_REGISTER, ///< Register and exit.
	CMD_LOGIN ///< Login and wait for instructions.
} umode_t;

/**
 * @brief Program configuration.
 * @details This struct keeps the configuration retrived by parsing program arguments at program start.
 */
typedef struct {
	char *username; ///< Username used for login or registration.
	char *password; ///< Password used for login or registration.
	umode_t mode; ///< Specifies whether to login or to register.
} options_t;

/**
 * @brief Parse program arguments for options.
 * @details First, the POSIX-compliant `getopt()` is used to parse named options, whereupon positional arguments are read. If the program was called violating the synopsis, a usage message is printed and the program is terminated.
 * @param argc The cardinality of `argv`.
 * @param argv The program argument vector.
 * @param options The options to store the parsed arguments in.
 */
void parse_arguments(int argc, char *argv[], options_t *options);

#endif
