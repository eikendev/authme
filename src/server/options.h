/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module declares functions to handle the server's argument parsing.
 * @details Argument parsing is done via `getopt()`.
 */

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

/**
 * @brief Program configuration.
 * @details This struct is used to keep the configuration retrived by parsing program arguments at program start.
 */
typedef struct {
	char *database_path; ///< Path of the file where the database is read from.
} options_t;

/**
 * @brief Parse program arguments for options.
 * @details The POSIX-compliant `getopt()` is used to parse named options. If the program was called violating the synopsis, a usage message is printed and the program is terminated.
 * @param argc The cardinality of `argv`.
 * @param argv The program argument vector.
 * @param options The options to store the parsed arguments in.
 */
void parse_arguments(int argc, char *argv[], options_t *options);

#endif
