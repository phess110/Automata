/*
* Author: Peter Hess
* File: NFA.c
* Date: 9/20/17
*
* Data stucture for a non-deterministic finite automaton.
*/


#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include "IntSet.h"
#include "nfa.h"

#define sigma 128
#define HALT IntSet_new()

/**
* Allocate and return a new NFA containing the given number of states.
*/
NFA* NFA_new(int nstates) {
	NFA* nfa = (NFA*)malloc(sizeof(NFA));
	(nfa->numStates) = nstates;

	(nfa->curr) = IntSet_new();										//Allocate a new IntSet for the current state, containing the state 0
	IntSet_add(nfa->curr, 0);	

	(nfa->accept) = (bool*)malloc(nstates * sizeof(bool));
	(nfa->tTable) = (IntSet **)malloc(nstates * sizeof(IntSet*));

	for (int i = 0; i < nstates; i++) {

		(nfa->tTable)[i] = (IntSet*)malloc(sigma * sizeof(IntSet));	//Each row of tTable will be an array of 128 ints
		(nfa->accept)[i] = false;									//Initially set all states to non-accepting

		for (int j = 0; j < sigma; j++) {
			(nfa->tTable)[i][j] = *HALT;							//Set all transitions to HALT (empty set), by default
		}

	}

	return nfa;
}

/**
* Free the given NFA.
*/
void NFA_free(NFA* nfa) {
	free(nfa -> accept);
	free(nfa->tTable);						
	free(nfa);
}

/**
* Return the number of states in the given NFA.
*/
int NFA_get_size(NFA* nfa) {
	return (nfa->numStates);
}

/**
* Return the set of next states specified by the given NFA's transition
* function from the given state on input symbol sym.
*/
IntSet* NFA_get_transitions(NFA* nfa, int state, char sym) {
	return &((nfa->tTable)[state][(int)sym]); //getting null ptrs
}

/**
* For the given NFA, add the state dst to the set of next states from
* state src on input symbol sym.
*/
void NFA_add_transition(NFA* nfa, int src, char sym, int dst) {
	IntSet_add(&((nfa -> tTable)[src][(int)sym]), dst);
}

/**
* Add a transition for the given NFA for each symbol in the given str.
*/
void NFA_add_transition_str(NFA* nfa, int src, char *str, int dst) {
	for (int i = 0; str[i] != '\0'; i++) {
		IntSet_add(&((nfa->tTable)[src][(int)str[i]]), dst);
	}
}

/**
* Add a transition for the given NFA for each input symbol.
*/
void NFA_add_transition_all(NFA* nfa, int src, int dst) {
	for (int i = 0; i < sigma; i++) {
		IntSet_add(&((nfa->tTable)[src][i]), dst);
	}
}

/**
* Set whether the given NFA's state is accepting or not.
*/
void NFA_set_accepting(NFA* nfa, int state, bool value) {
	(nfa->accept)[state] = value;
}

/**
* Return true if the given NFA's state is an accepting state.
*/
bool NFA_get_accepting(NFA* nfa, int state) {
	return (nfa->accept)[state];
}

/**
* Run the given NFA on the given input string, and return true if it accepts
* the input, otherwise false.
*/
bool NFA_execute(NFA* nfa, char *input) {
	for (int i = 0; input[i] != '\0'; i++) {
		IntSet* next = IntSet_new();
		
		IntSetIterator* iter = IntSet_iterator(nfa->curr);								//Iterate through current state.
		while (IntSetIterator_has_next(iter)) {
			IntSet_union(next, NFA_get_transitions(nfa, IntSetIterator_next(iter), input[i]) ); //Union transition from state in curr on char c to IntSet next
		}

		if (IntSet_is_empty(next)) { //Reject if no available transitions
			return false;
		}
		else {
			(nfa->curr) = next;
		}
	}
	IntSetIterator* iter = IntSet_iterator(nfa->curr);
	while (IntSetIterator_has_next(iter)) {
		if (NFA_get_accepting(nfa, IntSetIterator_next(iter))) {
			return true;
		}
	}
	return false;
}

/**
* Print the given NFA to System.out.
*/
void NFA_print(NFA* nfa) {
	printf("The set of states: 0, ... %d.\n", NFA_get_size(nfa) - 1);
	printf("The start state is 0.\n");
	printf("The transition function is given by the following transition table.\n");

	//Print tTable
	for (int i = 0; i < (nfa->numStates); i++) {
		printf("State: %d\t| ", i);					//Print state, followed by all possible transitions from that state
		for (int j = 1; j < sigma; j++) {
			if (IntSet_is_empty(&((nfa->tTable)[i][j])) == false) {
				printf("on \'%c\' to ", (char)j);
				IntSet_print(&((nfa->tTable)[i][j]));	//Print transition set from state i on input char j
				printf("\t | ");
			}
		}
		printf("halt on remaining inputs.\n");
	}

	printf("The accepting states are {");
	for (int i = 0; i < NFA_get_size(nfa); i++) {
		if ((nfa->accept)[i]) {
			printf("%d  ", i);
		}
	}
	printf("}.\n");
}
