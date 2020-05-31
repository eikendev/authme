/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module provides function definitions for basic list operations.
 * @details Basic list operations include list initialization, list destruction, and item manipulation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_initialize(void)
{
	list_t *list = malloc(sizeof(list_t));

	if (list == NULL)
		return NULL;

	list->head = NULL;
	list->length = 0;

	return list;
}

void list_destroy(list_t *list)
{
	if (list == NULL)
		return;

	element_t *next = list->head;

	for (element_t *curr = next; curr != NULL; curr = next) {
		next = curr->next;
		free(curr->data);
		free(curr);
	}

	free(list);
}

int list_size(list_t *list)
{
	return list->length;
}

bool list_add(list_t *list, obj_t obj, size_t n)
{
	if (obj == NULL)
		return false;

	obj_t inmem = malloc(n);
	if (inmem == NULL)
		return false;

	memcpy(inmem, obj, n);

	element_t *element = malloc(sizeof(element_t));
	if (element == NULL) {
		free(inmem);
		return false;
	}

	element->data = inmem;
	element->next = NULL;

	if (list->head == NULL) {
		list->head = element;
	} else {
		element_t *curr;
		for (curr = list->head; curr->next != NULL; curr = curr->next) {
		}
		curr->next = element;
	}

	list->length += 1;
	return true;
}

bool list_remove(list_t *list, obj_t obj)
{
	element_t *last = NULL;

	for (element_t *curr = list->head; curr != NULL; curr = curr->next) {
		if (curr->data == obj) {
			if (last != NULL)
				last->next = curr->next;
			else
				list->head = curr->next;

			free(curr->data);
			free(curr);
			list->length -= 1;
			return true;
		}

		last = curr;
	}

	return false;
}
