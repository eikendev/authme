/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function declarations for shared memory setup and cleanup.
 * @details Shared memory is used to transfer data between the clients and the server.
 */

#ifndef __SHMEM_H_SHARE__
#define __SHMEM_H_SHARE__

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Function to create and set up shared memory.
 * @details This function uses the global variable `shmem` as memory pointer.
 * @param name Filename of the shared memory.
 * @param len Size of the memory to create.
 * @param master Specifies whether to truncate the memory or not.
 * @return File descriptor or negative value in case of error.
 */
int create_shared_memory(char *name, size_t len, bool master);

/**
 * @brief Function to close an clean up a shared memory.
 * @details This function uses the global variable `shmem` as memory pointer.
 * @param name Filename of the shared memory.
 * @param len Size of the memory to unlink.
 * @param fd File descriptor that was used to open the shared memory.
 * @param master Specifies whether to unlink the memory or not.
 * @return `0` on success, negative value otherwise.
 */
int close_shared_memory(char *name, size_t len, int fd, bool master);

#endif
