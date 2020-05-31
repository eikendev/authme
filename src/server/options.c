/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module handles the servers's argument parsing.
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
	fprintf(stderr, "Usage: %s [ -l database ]\n", progname);
	exit(EXIT_FAILURE);
}

void parse_arguments(int argc, char *argv[], options_t *options)
{
	opterr = 0;

	bool parsed_database = false;

	int c;
	while ((c = getopt(argc, argv, "l:")) != -1) {
		switch (c) {
		case 'l':
			if (parsed_database)
				usage();

			options->database_path = optarg;
			parsed_database = true;
			break;
		default:
			usage();
		}
	}

	if (argc != optind)
		usage();

	if (!parsed_database)
		options->database_path = NULL;
}
