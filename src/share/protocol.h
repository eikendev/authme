/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains protocol specific definiations.
 * @details Protocol specific definitions include semaphore names, shared memory name and packet structs.
 */

#ifndef __PROTOCOL_H_SHARE__
#define __PROTOCOL_H_SHARE__

/**
 * @brief The filename of the shared memory.
 */
#define SHM_NAME "authme_auth"

/**
 * @brief The maximum size of the username field.
 */
#define MAX_USERNAME_LEN 32

/**
 * @brief The maximum size of the password field.
 */
#define MAX_PASSWORD_LEN 32

/**
 * @brief The maximum size of the secret field.
 */
#define MAX_SECRET_LEN 128

/**
 * @brief The length of a session id.
 */
#define SESSION_ID_SIZE 32

/**
 * @brief The size of the shared memory.
 */
#define SHM_LEN (10 + MAX_USERNAME_LEN + MAX_PASSWORD_LEN + MAX_SECRET_LEN + SESSION_ID_SIZE)

/**
 * @brief The name of the first server semaphore.
 */
#define SEM_SERVER1 "authme_server1"

/**
 * @brief The name of the second server semaphore.
 */
#define SEM_SERVER2 "authme_server2"

/**
 * @brief The name of the client semaphore.
 */
#define SEM_CLIENT1 "authme_client1"

/**
 * @brief The name of the exit semaphore.
 */
#define SEM_EXIT "authme_exit"

/**
 * @brief Enum for the server status.
 * @details Used to determine if server can currently respond.
 */
enum server_status_e {
	ONLINE, ///< Server can currently respond.
	OFFLINE ///< Server can currently not respond.
};

/**
 * @brief Enum for the request status.
 * @details Used to determine if the last request was successful.
 */
enum request_status_e {
	SUCCESS, ///< The request was successfully executed.
	ERROR ///< The request could not be fullfilled.
};

/**
 * @brief Enum for packet types.
 */
enum packet_e {
	REGISTRATION, ///< Packet to perform the registration of a user.
	LOGIN, ///< Packet to perform login of a user.
	LOGOUT, ///< Packet to perform logout of a user.
	SECRET_WRITE, ///< Packet to write a new secret to the database.
	SECRET_READ ///< Packet to read the stored secret.
};

/**
 * @brief Generic packet structure.
 * @details This structure is shared across all packets.
 */
struct packet_generic {
	enum server_status_e status; ///< Current server status.
	enum request_status_e rstatus; ///< Current request status.
	enum packet_e type; ///< Type of the packet.
};

/**
 * @brief Packet to perform the registration of a user.
 */
struct packet_registration {
	enum server_status_e status; ///< Current server status.
	enum request_status_e rstatus; ///< Current request status.
	enum packet_e type; ///< Type of the packet.
	char username[MAX_USERNAME_LEN + 1]; ///< Username of the user.
	char password[MAX_PASSWORD_LEN + 1]; ///< Password of the user.
};

/**
 * @brief Packet to perform login of a user.
 */
struct packet_login {
	enum server_status_e status; ///< Current server status.
	enum request_status_e rstatus; ///< Current request status.
	enum packet_e type; ///< Type of the packet.
	char username[MAX_USERNAME_LEN + 1]; ///< Username of the user.
	char password[MAX_PASSWORD_LEN + 1]; ///< Password of the user.
	char session_id[SESSION_ID_SIZE + 1]; ///< Session id of the client.
};

/**
 * @brief Packet to perform logout of a user.
 */
struct packet_logout {
	enum server_status_e status; ///< Current server status.
	enum request_status_e rstatus; ///< Current request status.
	enum packet_e type; ///< Type of the packet.
	char session_id[SESSION_ID_SIZE + 1]; ///< Session id of the client.
	char username[MAX_USERNAME_LEN + 1]; ///< Username of the user.
};

/**
 * @brief Packet to write a new secret to the database.
 */
struct packet_secret_write {
	enum server_status_e status; ///< Current server status.
	enum request_status_e rstatus; ///< Current request status.
	enum packet_e type; ///< Type of the packet.
	char session_id[SESSION_ID_SIZE + 1]; ///< Session id of the client.
	char username[MAX_USERNAME_LEN + 1]; ///< Username of the user.
	char secret[MAX_SECRET_LEN + 1]; ///< Secret to write to the database.
};

/**
 * @brief Packet to read the stored secret.
 */
struct packet_secret_read {
	enum server_status_e status; ///< Current server status.
	enum request_status_e rstatus; ///< Current request status.
	enum packet_e type; ///< Type of the packet.
	char session_id[SESSION_ID_SIZE + 1]; ///< Session id of the client.
	char username[MAX_USERNAME_LEN + 1]; ///< Username of the user.
	char secret[MAX_SECRET_LEN + 1]; ///< Secret read from the database.
};

#endif
