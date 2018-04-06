/*
* Author: Peter Hess
* File: Auto.c
* Date: 9/20/17
*/

#ifndef _auto_h
#define _auto_h

extern void onlyAB();

extern void startsWithAB();

extern void evenOnes();

extern void evenOnesZeros();

extern void bNoA();

extern void endInMAN();

extern void washington();

extern void xyz();

extern void getUserInputDFA(DFA* dfa);

extern void getUserInputNFA(NFA* nfa);

extern DFA* subsetConstruct(NFA* nfa);

#endif