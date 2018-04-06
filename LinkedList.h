#ifndef _LL_h
#define _LL_h

#include <stdbool.h>

typedef struct LL LL;

/**
* Allocate, initialize and return a new (empty) LL.
*/
extern LL *LL_new();

/**
* Free the memory used for the given LL.
* If boolean free_data_also is true, also free the data associated with
* each element.
*/
extern void LL_free(LL *list, bool free_data_also);

/**
* Return true if the given LL is empty.
*/
extern bool LL_is_empty(const LL *list);

/**
* Add the given void* value at the front of the given LL.
*/
extern void LL_add_at_front(LL *list, void *value);

/**
* Add the given void* value at the end of the given LL.
*/
extern void LL_add_at_end(LL *list, void *value);

/**
* Return true if then given LL contains given void* value.
*/
extern bool LL_contains(const LL *list, void *value);

/**
* Remove the given void* value from the given LL if it is there.
* Note that this does not free the data associated with the element.
*/
extern void LL_remove(LL *list, void *data);

/**
* Return the void* value at the given index in the given LL, or
* NULL if there is no such.
* Note that this means you can't store NULL in a LL. C'est la vie.
*/
extern void *LL_element_at(LL *list, int index);

/**
* Remove and return the first element from the given LL.
* Returns NULL if the list is empty.
*/
extern void *LL_pop(LL *list);

extern int get_size(LL* LL);

/**
* Call the given function on each element of given LL, passing the
* void* value to the function.
*/
extern void LL_iterate(const LL *list, void(*func)(void*));

// Partial declaration
typedef struct LLNode *LLIterator;

/**
* Return an LLIterator for the given LL.
* Don't forget to free() this when you're done iterating.
*/
extern LLIterator *LL_iterator(const LL *list);

/**
* Return true if the given LLIterator will return another element
* if LLIterator_next() is called.
*/
extern bool LLIterator_has_next(const LLIterator *iterator);

/**
* Return the next value from the given LLIterator and increment it
* to point to the next element.
* Will return NULL if there is no such element.
* This means that you can't store NULL in a LL. C'est la vie.
*/
extern void *LLIterator_next(LLIterator *iterator);

extern void LL_print_string_list(LL *list);

#endif
