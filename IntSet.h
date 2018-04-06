/*
* File: IntSet.h
* Creator: George Ferguson
* Created: Fri Jul  1 09:41:51 2016
* Time-stamp: <Fri Aug  4 09:31:27 EDT 2017 ferguson>

* Note: Minor edits have been made to ensure compatability
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef _IntSet_h
#define _IntSet_h

typedef struct IntSet {
	unsigned long long bits;
}IntSet;

typedef struct IntSetIterator {
	IntSet* set;
	int index;
}IntSetIterator;

extern IntSet* IntSet_new();

extern void IntSet_free(IntSet* set);

bool IntSet_is_empty(IntSet* set);

void IntSet_add(IntSet* set, int value);

bool IntSet_contains(const IntSet* set, int value);

void IntSet_union(IntSet* set1, const IntSet* set2);

bool IntSet_equals(IntSet* set1, IntSet* set2);

IntSetIterator* IntSet_iterator(IntSet* set);

bool IntSetIterator_has_next(IntSetIterator* iterator);

int IntSetIterator_next(IntSetIterator* iterator);

void IntSet_print(IntSet* set);

#endif