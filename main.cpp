/** ***************************************************************************
 * @assignment: Final Project
 * @file    main.cpp
 * @author  Zach Gassner & Justin Blechel
 * @date    Revised: 3 May 2023
 *
 * @desc Implement an algorithm to minimize a DFA.
 *  The standard algorithm that runs in O(m*n^2) time can be improved.
 *  One such faster algorithm is provided in an earlier version of our text book.
 *
 *  Minimizing means reducing the number of states in DFA.
 *  We must detect the states whose presence or absence
 *  in the DFA does not affect language accepted by DFA.
 *  These states can be eliminated from the DFA.
 *
 * @remark A DFA (deterministic finite automaton) 'alphabet_size' is defined by a 5-tuple:
 *      alphabet_size = <Q, Σ, δ, q₀, F>
 *      - Q: finite set of states
 *      – Σ: a finite set of input symbols (alphabet)
 *      – δ: transition function δ: Q x Σ -> Q
 *      – q₀ ∈ Q: initial state
 *      – F ⊆ Q: set of accepting states
 *
 * @remark Let w = a₁a₂…aₙ be a string over the alphabet Σ.
 *  The automaton alphabet_size accepts the string w if a sequence of states,
 *  r₀,r₁,…,rₙ, exists in Q with the following conditions:
 *  1. r₀ = q₀
 *  2. rᵢ₊₁ = δ(rᵢ, aᵢ₊₁), for i = 0, …,n − 1
 *  3. rₙ ∈ F
 *
 *****************************************************************************/

#include <iostream>
#include "DFA.hpp"


int main(int argc, char* argv[]) {

    for (int i = 1; i < argc; i++) {
        std::cout << "======================\n";
        std::string fileName = argv[i];
        DFA dfa(fileName);
        dfa.removeUnreachableStates();
        dfa.minimize();
        fileName.insert(fileName.find('.'), "-minimized");
        std::cout << "MINIMIZED ";
        dfa.printToConsole(i);
        dfa.printToFile(fileName);
        if (i == argc) {

        }
    }
    std::cout << "======================";
    return 0;
}