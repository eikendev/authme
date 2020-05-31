/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module handles the clients's argument parsing.
 * @details Argument parsing is done via `getopt()`. A usage message is printed if the synopsis is violated.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "options.h"

/**
 * @brief Program name.
 * @details This variable must be set on program start.
 */
extern char *progname;

/**
 * @brief Print a usage message.
 * @details The function terminates the program with the value `EXIT_FAILURE`. The global variable `progname` has to be defined in order for this function to work.
 */
static void usage(void)
{
	fprintf(stderr, "Usage: %s { -r | -l } <username> <password>\n", progname);
	exit(EXIT_FAILURE);
}

void parse_arguments(int argc, char *argv[], options_t *options)
{
	opterr = 0;

	bool parsed_register = false;
	bool parsed_login = false;

	int c;
	while ((c = getopt(argc, argv, "rl")) != -1) {
		switch (c) {
		case 'r':
			if (parsed_register)
				usage();

			options->mode = CMD_REGISTER;
			parsed_register = true;
			break;
		case 'l':
			if (parsed_login)
				usage();

			options->mode = CMD_LOGIN;
			parsed_login = true;
			break;
		default:
			usage();
		}
	}

	if ((parsed_register && parsed_login) ||
		(!parsed_register && !parsed_login))
		usage();

	if (argc - optind != 2)
		usage();

	options->username = argv[optind++];
	options->password = argv[optind++];
}
