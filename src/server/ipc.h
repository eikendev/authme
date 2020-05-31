/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function declarations for interprocess communication.
 * @details The functions include the ability to handle packets from clients.
 */

#ifndef __IPC_H__
#define __IPC_H__

#include "list.h"

#include "../share/protocol.h"

/**
 * @brief Representation of a client.
 */
typedef struct {
	char session_id[SESSION_ID_SIZE + 1]; ///< Session id for the client.
	char username[MAX_USERNAME_LEN + 1]; ///< Username of the client.
} client_t;

/**
 * @brief Handle the packet `packet`.
 * @details Inspects the packet type and delegates to the specific packet handler.
 * @param packet The packet to handle.
 */
void handle_packet(void *packet);

#endif
