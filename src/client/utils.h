/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains the client utility function declarations.
 * @details Functions in this module are not program specific.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "semaphore.h"

/**
 * @brief Perform a checked semaphore wait.
 * @details Checked in this sense means to make sure that the server has not been terminated yet. Makes use of the global variable `shmem`.
 * @param sem The semaphore to wait for.
 */
void sem_wait_checked(sem_t *sem);

/**
 * @brief Perform a checked semaphore post.
 * @details Checked in this sense means to make sure that the server has not been terminated yet. Makes use of the global variable `shmem`.
 * @param sem The semaphore to post.
 */
void sem_post_checked(sem_t *sem);

#endif
