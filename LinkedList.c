#include <stdio.h>
#include <stdlib.h>
#include "LL.h"

struct LL {
	struct LLNode *first;
	struct LLNode *last;
	int size;
};

typedef struct LLNode {
	void *data;
	struct LLNode *next;
	struct LLNode *prev;
} LLNode;

LL * LL_new() {
	LL *list = (LL*)malloc(sizeof(LL));
	list->first = list->last = NULL;
	list->size = 0;
	return list;
}

int get_size(LL* LL) {
	return LL->size;
}

static LLNode * LLNode_new(void *data) {
	LLNode *node = (LLNode*)malloc(sizeof(LLNode));
	if (node == NULL) {
		abort();
	}
	node->data = data;
	node->next = node->prev = NULL;
	return node;
}

void LL_free(LL *list, bool free_data_also) {
	// Free the elements
	LLNode *elt = list->first;
	while (elt != NULL) {
		LLNode *next = elt->next;
		if (free_data_also && elt->data != NULL) {
			free(elt->data);
		}
		free(elt);
		elt = next;
	}
	// Free the list itself
	free(list);
}

bool LL_is_empty(const LL *list) {
	return list->first == NULL;
}

void LL_add_at_front(LL *list, void *data) {
	LLNode *node = LLNode_new(data);
	node->next = list->first;
	if (list->first != NULL) {
		list->first->prev = node;
	}
	list->first = node;
	if (list->last == NULL) {
		list->last = node;
	}
	list->size += 1;
}


void LL_add_at_end(LL *list, void *data) {
	LLNode *node = LLNode_new(data);
	node->prev = list->last;
	if (list->last != NULL) {
		list->last->next = node;
	}
	list->last = node;
	if (list->first == NULL) {
		list->first = node;
	}
	list->size += 1;
}


bool LL_contains(const LL *list, void *data) {
	for (LLNode *node = list->first; node != NULL; node = node->next) {
		if (node->data == data) {
			return true;
		}
	}
	return false;
}


void LL_remove(LL *list, void *data) {
	for (LLNode *node = list->first; node != NULL; node = node->next) {
		if (node->data == data) {
			if (node == list->first) {
				list->first = node->next;
			}
			if (node == list->last) {
				list->last = node->prev;
			}
			if (node->prev != NULL) {
				node->prev->next = node->next;
			}
			if (node->next != NULL) {
				node->next->prev = node->prev;
			}
			free(node);
			return;
		}
	}
}


void * LL_element_at(LL *list, int index) {
	int i = 0;
	for (LLNode *node = list->first; node != NULL; node = node->next) {
		if (i == index) {
			return node->data;
		}
		i += 1;
	}
	return NULL;
}


void * LL_pop(LL *list) {
	void *data = LL_element_at(list, 0);
	if (data != NULL) {
		LL_remove(list, data); // Removes first occurrence
	}
	return data;
}


void LL_iterate(const LL *list, void(*func)(void *)) {
	for (LLNode *node = list->first; node != NULL; node = node->next) {
		func(node->data);
	}
}

/**
* Return an LLIterator for the given LL.
* Don't forget to free() this when you're done iterating.
*/
LLIterator *
LL_iterator(const LL *list) {
	LLIterator *iterator = (LLIterator*)malloc(sizeof(LLIterator));
	*iterator = list->first;
	return iterator;
}

/**
* Return true if the given LLIterator will return another element
* if LLIterator_next() is called.
*/
bool
LLIterator_has_next(const LLIterator *iterator) {
	return iterator != NULL && *iterator != NULL;
}

/**
* Return the next value from the given LLIterator and increment it
* to point to the next element.
* Will return NULL if there is no such element.
* This means that you can't store NULL in a LL. C'est la vie.
* It would be easy to allow it and signal `no such element' some other way...
*/
void *
LLIterator_next(LLIterator *iterator) {
	if (iterator == NULL || *iterator == NULL) {
		return NULL;
	}
	else {
		void *data = (*iterator)->data;
		*iterator = (*iterator)->next;
		return data;
	}
}

/**
* Print the given LL to stdout, assuming that the values are
* all null-terminated strings.
*/
void
LL_print_string_list(LL *list) {
	for (LLNode *node = list->first; node != NULL; node = node->next) {
		printf("%s", (char*)node->data);
		if (node->next != NULL) {
			printf(" ");
		}
	}
	printf("\n");
}
