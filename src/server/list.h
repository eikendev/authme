/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module provides function declarations for basic list operations.
 * @details Basic list operations include list initialization, list destruction, and item manipulation.
 */

#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Generic object type for the database.
 */
typedef void *obj_t;

/**
 * @brief Entry representation of the database.
 */
typedef struct element_s {
	obj_t data; ///< Data block of this entry.
	struct element_s *next; ///< Next element in the list.
} element_t;

/**
 * @brief Representation of a linked list.
 */
typedef struct {
	element_t *head; ///< Head element of the list.
	int length; ///< Length of the list.
} list_t;

/**
 * @brief Create a new list.
 * @return The memory address of the list.
 */
list_t *list_initialize(void);

/**
 * @brief Destroy the list `list` created previously.
 * @details This function is called to free allocated memory.
 * @param list The list to destroy.
 */
void list_destroy(list_t *list);

/**
 * @brief Returns the size of the list `list`.
 * @param list The list to get the size of.
 * @return The length of the list.
 */
int list_size(list_t *list);

/**
 * @brief Add an object to the list `list`.
 * @param list The list to add the item to.
 * @param obj The item to add to the list.
 * @param n The length of the item `obj`.
 * @return `true` on success, `false` on failure.
 */
bool list_add(list_t *list, obj_t obj, size_t n);

/**
 * @brief Remove the item `obj` from the list `list`.
 * @param list The list to remove the item from.
 * @param obj The item to remove from the list.
 * @return `true` on success, `false` on failure.
 */
bool list_remove(list_t *list, obj_t obj);

#endif
