/*
* File: IntSet2.c
* Creator: George Ferguson
* Created: Thu Aug  3 17:36:24 2017
* Time-stamp: <Tue Aug  8 10:11:39 EDT 2017 ferguson>
*
* IntSet implemented as a bit vector.
* Using long long should allow values from 0 to 63.
*
* Note (PH): Minor edits have been made to ensure compatability.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "IntSet.h"

IntSet* IntSet_new() {
	IntSet* set1 = (IntSet*)malloc(sizeof(IntSet));
	(set1->bits) = 0;
	return set1;
}

void IntSet_free(IntSet* set) {
	if (set != NULL) {
		free(set);
	}
}

bool IntSet_is_empty(IntSet* set) {
	return ((set->bits) == 0);
}

void IntSet_add(IntSet* set, int value) {
	if (value < 0 || value >= 32) {
		fprintf(stderr, "IntSet_add: value out of range: %d\n", value);
		abort();
	}
	(set->bits) |= (1L << value);
}

bool IntSet_contains(const IntSet* set, int value) {
	return (set->bits) & (1L << value);
}

void IntSet_union(IntSet* set1, const IntSet* set2) {
	set1->bits |= set2->bits;
}

bool IntSet_equals(IntSet* set1, IntSet* set2) {
	return (set1->bits) == (set2->bits);
}

IntSetIterator* IntSet_iterator(IntSet* set) {
	IntSetIterator* iterator = (IntSetIterator*)malloc(sizeof(IntSetIterator));
	(iterator->index) = 0;
	(iterator->set) = set;
	return iterator;
}

bool IntSetIterator_has_next(IntSetIterator* iterator) {
	while ((iterator->index) < 32) {
		if (IntSet_contains(iterator->set, iterator->index)) {
			return true;
		}
		else {
			(iterator->index) = (iterator->index) + 1;
		}
	}
	return false;
}

int IntSetIterator_next(IntSetIterator* iterator) {
	int value = iterator->index;
	(iterator->index) += 1;
	return value;
}

void IntSet_print(IntSet* set) {
	IntSetIterator* iterator = IntSet_iterator(set);
	printf("{");
	while (IntSetIterator_has_next(iterator)) {
		int value = IntSetIterator_next(iterator);
		printf("%d,", value);
	}
	printf("}\n");
	free(iterator);
}