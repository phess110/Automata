/*
* Author: Peter Hess
* File: DFA.c
* Date: 9/20/17
*
* Data stucture for a deterministic finite automaton.
*/

#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include "dfa.h"

#define HALT -1
#define sigma 128 

/**
* Allocate and return a new DFA containing the given number of states.
*/
DFA* DFA_new(int n) {

	DFA* dfa = (DFA*)malloc(sizeof(DFA));
	(dfa->numStates) = n;
	(dfa->curr) = 0;

	(dfa->accept) = (bool*)malloc(n * sizeof(bool));					//accept is an array of n booleans
	(dfa->tTable) = (int**)malloc(n * sizeof(int*));					//tTable is an n by 128 dimensional array

	for (int i = 0; i < n; i++) {

		(dfa->tTable)[i] = (int *)malloc(sigma * sizeof(int));		//Each row of tTable will be an array of 128 ints
		(dfa->accept)[i] = false;									//Initially set all states to non-accepting

		for (int j = 0; j < sigma; j++) {
			(dfa->tTable)[i][j] = HALT;								//Set all transitions to HALT, by default
		}

	}

	return dfa;
}

/**
* Free the given DFA.
*/
void DFA_free(DFA* dfa) {
	free(dfa->accept);
	for (int i = 0; i < DFA_get_size(dfa); i++) {
		free((dfa->tTable)[i]);
	}
	free(dfa->tTable);
	free(dfa);
}

/**
* Return the number of states in the given DFA.
*/
int DFA_get_size(DFA* dfa) {
	return (dfa->numStates);
}

/**
* Return the state specified by the given DFA's transition function from
* state src on input symbol sym.
*/
int DFA_get_transition(DFA* dfa, int src, char sym) {
	return (dfa->tTable)[src][(int)sym];
}

/**
* For the given DFA, set the transition from state src on input symbol
* sym to be the state dst.
*/
void DFA_set_transition(DFA* dfa, int src, char sym, int dst) {
	(dfa->tTable)[src][(int)sym] = dst;
}

/**
* Set the transitions of the given DFA for each symbol in the given str.
* This is a nice shortcut when you have multiple labels on an edge between
* two states.
*/
void DFA_set_transition_str(DFA* dfa, int src, char *str, int dst) {
	for (int i = 0; str[i] != '\0'; i++) {
		(dfa->tTable)[src][(int)str[i]] = dst;
	}
}

/**
* Set the transitions of the given DFA for all input symbols.
* Another shortcut method.
*/
void DFA_set_transition_all(DFA* dfa, int src, int dst) {
	for (int i = 0; i < sigma; i++) {
		(dfa->tTable)[src][i] = dst;
	}
}

/**
* Set whether the given DFA's state is accepting or not.
*/
void DFA_set_accepting(DFA* dfa, int state, bool value) {
	(dfa->accept)[state] = value;
}

/**
* Return true if the given DFA's state is an accepting state.
*/
bool DFA_get_accepting(DFA* dfa, int state) {
	return (dfa->accept)[state];
}

/**
* Run the given DFA on the given input string, and return true if it accepts
* the input, otherwise false.
*/
bool DFA_execute(DFA* dfa, char *input) {
	int next;
	for (int i = 0; input[i] != '\0'; i++) {

		next = DFA_get_transition(dfa, dfa->curr, input[i]); //Get transition on input char
		if (next == HALT) {
			return false;		//Reject if no transition is available
		}
		else {
			(dfa->curr) = next; //Set next as current state
		}
	}

	if ((dfa->accept)[dfa->curr] == true) { //Accept string if in accepting state
		return true;
	}
	return false;
}

/**
* Print the given DFA to System.out.
*/
void DFA_print(DFA* dfa) {

	printf("The set of states 0,...,%d.\n", DFA_get_size(dfa) - 1);
	printf("The start state is 0.\n");
	printf("The transition function is given by the following transition table.\n");

	//Print tTable 
	for(int n = 0; n < DFA_get_size(dfa); n++){
		printf("State: %d\t| ", n);
		for (int i = 0; i < sigma; i++) {
			int trans = DFA_get_transition(dfa, n, (char)i);
			if (trans != HALT) {
				printf("on \'%c\' goto %d \t| ", (char)i, trans);
			}
		}
		printf("halt on remaining inputs.\n");
	}

	//Print accepting states
	printf("The accepting states are {");
	for (int i = 0; i < DFA_get_size(dfa); i++) {
		if ((dfa->accept)[i]) {
			printf("%d  ", i);
		}
	}
	printf("}.\n");
}