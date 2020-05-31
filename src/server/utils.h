/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains the server utility function declarations.
 * @details Functions in this module are not program specific.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * @brief Set the server status flag to online.
 * @param mem Memory where the flag is set in.
 */
void set_status_online(void *mem);

/**
 * @brief Set the server status flag to offline.
 * @param mem Memory where the flag is set in.
 */
void set_status_offline(void *mem);

#endif
