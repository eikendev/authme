/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains the server utility function definitions.
 * @details Functions in this module are not program specific.
 */

#include <stddef.h>

#include "../share/protocol.h"

void set_status_online(void *mem)
{
	if (mem != NULL) {
		struct packet_generic *p = mem;
		p->status = ONLINE;
	}
}

void set_status_offline(void *mem)
{
	if (mem != NULL) {
		struct packet_generic *p = mem;
		p->status = OFFLINE;
	}
}
