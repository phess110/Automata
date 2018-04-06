/*
* Author: Peter Hess
* File: Auto.c
* Date: 9/20/17
*
* Hardwires examples for the DFA and NFA data structure.
* Implements the subset construction algorithm.
* Gets user input to test DFA/NFA examples.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dfa.h"
#include "nfa.h"
#include "IntSet.h"
#include "LinkedList.h"
#include "Auto.h"

void getUserInputDFA(DFA* dfa);
void getUserInputNFA(NFA* nfa);
DFA* subsetConstruct(NFA* nfa);

//DFA to accept the string "ab" (case-sensitive)
void onlyAB() {
	printf("This DFA accepts only the string \"ab\". (Case-sensitive)\n");
	DFA* dfa = DFA_new(3);
	DFA_set_transition(dfa, 0, 'a', 1);
	DFA_set_transition(dfa, 1, 'b', 2);
	DFA_set_accepting(dfa, 2, true);
	DFA_print(dfa);
	getUserInputDFA(dfa);
	DFA_free(dfa);
	return;
}

//DFA accepts strings starting with "ab" (case-sensitive)
void startsWithAB() {
	printf("This DFA accepts strings starting with \"ab\". (Case-sensitive)\n");
	DFA* dfa = DFA_new(3);
	DFA_set_transition(dfa, 0,'a', 1);
	DFA_set_transition(dfa, 1, 'b', 2);
	DFA_set_transition_all(dfa, 2, 2);
	DFA_set_accepting(dfa, 2, true);
	getUserInputDFA(dfa);
	DFA_free(dfa);
	return;
}

//DFA accepts binary strings with an even number of ones (rejects if not a binary string)
void evenOnes() {
	printf("This DFA accepts only binary strings with an even number of 1s.\n");
	DFA* dfa = DFA_new(2);
	DFA_set_transition(dfa, 0, '0', 0);
	DFA_set_transition(dfa, 0, '1', 1);
	DFA_set_transition(dfa, 1, '1', 0);
	DFA_set_transition(dfa, 1, '0', 1);
	DFA_set_accepting(dfa, 0, true);
	getUserInputDFA(dfa);
	DFA_free(dfa);
	return;
}

//DFA accepts binary strings with an even number of ones and zeros (rejects if not a binary string)
void evenOnesZeros() {
	printf("This DFA accepts binary strings with an even number of 0s and 1s.\n");
	DFA* dfa = DFA_new(4);
	DFA_set_transition(dfa, 0, '0', 2);
	DFA_set_transition(dfa, 0, '1', 1);
	DFA_set_transition(dfa, 1, '1', 0);
	DFA_set_transition(dfa, 1, '0', 3);
	DFA_set_transition(dfa, 2, '0', 0);
	DFA_set_transition(dfa, 2, '1', 3);
	DFA_set_transition(dfa, 3, '1', 2);
	DFA_set_transition(dfa, 3, '0', 1);
	DFA_set_accepting(dfa, 0, true);
	getUserInputDFA(dfa);
	DFA_free(dfa);
	return;
}

//DFA accepts strings that contain at least one 'b', but that also contain exactly 0 'a's (case-sensitive)
void bNoA(){
	printf("This DFA accepts strings that contain at least one \'b\' and exactly 0 \'a\'s (Case-sensitive)\n");
	DFA* dfa = DFA_new(3);
	DFA_set_transition_all(dfa, 0, 0);
	DFA_set_transition(dfa, 0, 'a', 1);
	DFA_set_transition(dfa, 0, 'b', 2);
	DFA_set_transition_all(dfa, 2, 2);
	DFA_set_transition(dfa, 2, 'a', 1);
	DFA_set_accepting(dfa, 2, true);
	getUserInputDFA(dfa);
	DFA_free(dfa);
	return;
}

//NFA that accepts strings ending in "man" (case-sensitive)
void endInMAN() {
	printf("This NFA accepts strings ending in \"man\". (Case-sensitive)\n");
	NFA* nfa = NFA_new(4);
	NFA_add_transition_all(nfa, 0, 0);
	NFA_add_transition(nfa, 0, 'm', 1);
	NFA_add_transition(nfa, 1, 'a', 2);
	NFA_add_transition(nfa, 2, 'n', 3);
	NFA_set_accepting(nfa, 3, true);
	getUserInputNFA(nfa);
	DFA* manToDFA = subsetConstruct(nfa);
	printf("Equivalent DFA:\n");
	getUserInputDFA(manToDFA);
	DFA_free(manToDFA);
	NFA_free(nfa);
	return;
}

//NFA that accepts strings that end in x, y, or z, but don't start with x, y, or z (case-sensitive)
void xyz() {
	printf("This NFA accepts strings that end with an \'x\', \'y\', or \'z\' but that don't start with those letters. (Case-sensitive)\n");
	NFA* nfa = NFA_new(4);
	for (int i = 0; i < 128; i++) {
		if ((char)i != 'x' && (char)i != 'y' && (char)i != 'z') { //Transition from 0 to 2 on Lambda - {x,y,z}
			NFA_add_transition(nfa, 0, (char)i, 2);
		}
	}
	NFA_add_transition_str(nfa, 0, "xyz", 1);
	NFA_add_transition_str(nfa, 2, "xyz", 3);
	NFA_add_transition_all(nfa, 2, 2);
	NFA_set_accepting(nfa, 3, true);
	getUserInputNFA(nfa); 
	DFA* xyzToDFA = subsetConstruct(nfa);
	printf("Equivalent DFA:\n");
	getUserInputDFA(xyzToDFA);
	DFA_free(xyzToDFA);
	NFA_free(nfa);
	return;
}

//NFA accepts strings with more than one 'w', 'a', 's', 'h', 'i', 'o', 'g', 't' or more than two 'n' (case-sensitive)
void washington(){
	printf("NFA accepts strings with more than one \'w\', \'a\', \'s\', \'h\', \'i\', \'o\', \'g\', \'t\' or more than two 'n'\n");
	NFA* nfa = NFA_new(12);
	for (int i = 0; i < 12; i++) {
		NFA_add_transition_all(nfa, i, i);
	}
	char * str = "aghiostw";
	for (int i = 0; str[i] != '\0'; i++) {
		NFA_add_transition(nfa, 0, str[i], i+1);
		NFA_add_transition(nfa, i+1, str[i], 9);
	}
	NFA_add_transition(nfa, 0, 'n', 10);
	NFA_add_transition(nfa, 10, 'n', 11);
	NFA_add_transition(nfa, 11, 'n', 9);
	NFA_set_accepting(nfa, 9, true);
	getUserInputNFA(nfa);
	
	DFA* washToDFA = subsetConstruct(nfa);
	printf("Equivalent DFA:\n");
	getUserInputDFA(washToDFA);
	DFA_free(washToDFA);
	NFA_free(nfa);
	return;
}

//Requests user input, executes input on given string
void getUserInputDFA(DFA* dfa) {
	char str[20];
	printf("Enter an input string: ");
	scanf("%s", str);
	while (strcmp(str, "STOP") != 0) {
		if (DFA_execute(dfa, str)) {
			printf("Accepted.\n");
		}
		else {
			printf("Rejected.\n");
		}
		printf("Enter an input string: ");
		scanf("%s", str);
		dfa->curr = 0;
	}
}

//Requests user input, executes input on given string
void getUserInputNFA(NFA* nfa) {
	char str[20];
	printf("Enter an input string: ");
	scanf("%s", str);
	while (strcmp(str, "STOP") != 0) {
		if (NFA_execute(nfa, str)) {
			printf("Accepted.\n");
		}
		else {
			printf("Rejected.\n");
		}
		printf("Enter an input string: ");
		scanf("%s", str);
		IntSet* reset = IntSet_new();
		IntSet_add(reset, 0);
		nfa->curr = reset;
	}
}

//DFA state: contains a set of states of the nfa, a label (int i), and a boolean (whether the state is accepting or not)
typedef struct{
	IntSet* val;
	int i;
	bool toAccept;
}dfaState;

//DFA transition: contains a current state with a transition to next on input
typedef struct {
	int curr;
	char input;
	int next;
}dfaTrans;

dfaState* dfaState_new(int n, IntSet* s, bool acc) {
	dfaState* l = (dfaState*)malloc(sizeof(dfaState));
	(l->i) = n;
	(l->val) = s;
	(l->toAccept) = acc;
	return l;
}

dfaTrans* dfaTrans_new(int src, int dst, char sym) {
	dfaTrans* l = (dfaTrans*)malloc(sizeof(dfaTrans));
	(l->curr) = src;
	(l->input) = sym;
	(l->next) = dst;
	return l;
}

/*
* Function which takes an NFA as input and outputs an equivalent DFA, that is a DFA that accepts the same language.
* Uses the subset construction algorithm. 
*/
DFA* subsetConstruct(NFA* nfa) {
	IntSet* current = IntSet_new();			
	IntSet_add(current, 0);
	int numStates = 0;

	LinkedList* states = LinkedList_new();	//List of DFA states (sets of states of the NFA)
	LinkedList* trans = LinkedList_new();	//List of transitions between states.
	LinkedList_add_at_end(states, dfaState_new(numStates++, current, (nfa->accept)[0])); //add {0} state

	LinkedListIterator* iter1 = LinkedList_iterator(states);

	int currIndex = 0;
	while (currIndex < get_size(states)) {
		dfaState* currLLState = ((dfaState*)LinkedList_element_at(states, currIndex));

		for (int sym = 0; sym < sigma; sym++) {								//Iterate over alphabet

			IntSet* dst = IntSet_new();										//Destination state
			LinkedListIterator* iter2 = LinkedList_iterator(states);		//Need iterator to test whether 'dst' is already in states LL
			IntSetIterator* currIter = IntSet_iterator(currLLState->val);	//Iterator for next dfaState in LL
			bool dstAccept = false;											//True if dst contains an nfa accepting state (so state will be accepting in the dfa)

			while (IntSetIterator_has_next(currIter)) {						
				int temp = IntSetIterator_next(currIter);					//Iterate through nfa states in current set of states
				IntSet_union(dst, &((nfa->tTable)[temp][sym]));				//Union together all possible states on a given symbol
			}

			if (IntSet_is_empty(dst) == false) {	//If dst isn't empty, then there are available transitions on sym; else nothing should be done
				
				bool isContained = false;						//Determine whether dst is already in states LL (using iter2)
				int transDest;									//Name of destination state in dfa

				while (LinkedListIterator_has_next(iter2)) { 

					dfaState* tempState = (dfaState*)LinkedListIterator_next(iter2);
					if (IntSet_equals(dst, tempState->val) == true) {		//tests if dst is in 
						isContained = true;			
						//printf("FLAG TRUE");
						transDest = tempState->i;					//set transition destination to matched state
						break;
					}
				}

				if (isContained == false) {							//if dst is a new state, then add it to the states LL
					IntSetIterator* i = IntSet_iterator(dst);
					while (IntSetIterator_has_next(i)) {
						int n = IntSetIterator_next(i);
						if ((nfa->accept)[n]) {
							dstAccept = true;
						}
					}
					LinkedList_add_at_end(states, dfaState_new(numStates++, dst, dstAccept));
					transDest = numStates - 1;						//keep track of transition
				}
				LinkedList_add_at_end(trans, dfaTrans_new(currLLState->i, transDest, sym));		//Add transition from currLLState to transDest on character sym
			}
			free(iter2);
			free(currIter);
		}
		currIndex++;
	}
	free(iter1);
	DFA* dfa = DFA_new(numStates);					//create dfa with numStates total states
	LinkedListIterator* iter3 = LinkedList_iterator(states);

	while (LinkedListIterator_has_next(iter3)) {	//iterate through states LL and create the accept arr
		dfaState* dfaS = (dfaState*)LinkedListIterator_next(iter3);
		DFA_set_accepting(dfa, dfaS->i, dfaS->toAccept);
	}
	free(iter3);

	LinkedListIterator* iter4 = LinkedList_iterator(trans);
	while (LinkedListIterator_has_next(iter4)) {	//iterate through trans LL and create tTable
		dfaTrans* dfaT = (dfaTrans*)LinkedListIterator_next(iter4);
		DFA_set_transition(dfa, dfaT->curr, dfaT->input, dfaT->next);
	}
	free(iter4);
	LinkedList_free(states, true);
	LinkedList_free(trans, true);
	return dfa;
}

int main() {
	printf("Enter \"STOP\" to proceed to next DFA/NFA.\n");
	//DFAs 1-5
	onlyAB();
	startsWithAB();
	evenOnes();
	evenOnesZeros();
	bNoA();
	//NFAs 1-3
	endInMAN();
	washington();
	xyz();
	return 0;
}