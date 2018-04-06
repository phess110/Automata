# Automata

Implements data structures for deterministic finite automata (DFA) and non-deterministic autamata (NFA). Auto.c contains various instances of NFAs and DFAs, as well as an implementation of the subset construction algorithm for converting an NFA to a DFA.

The DFA consists of a number of states *n*, a current state *q*, a set of accepting states *F*, and a transition table (transition function *T*), which given *q* and an input symbol *w*, maps to a new state *q'*. The NFA is implemented similarly, however, it maintains a set of possible current states. On a given input symbol *w*, the NFA maps the set of current states *S* onto *T(S,w)*, the set of all states reachable from a state in *S* on input *w*. Thus, the execution of the NFA merely simulates non-determinism.
