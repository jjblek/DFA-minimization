/** ***************************************************************************
 * @assignment: Final Project
 * @file        main.cpp
 * @author      Zach Gassner & Justin Blechel
 * @date        Revised: 3 May 2023
 *
 * @desc Implement an algorithm to minimize a DFA.
 *  The standard algorithm that runs in O(m*n^2) time can be improved.
 *  One such faster algorithm is provided in an earlier version of our text book.
 *
 *  @remark Minimizing means reducing the number of states in DFA.
 *   We must detect the states whose presence or absence
 *   in the DFA does not affect language accepted by DFA.
 *   These states can be eliminated from the DFA.
 *
 * @remark A DFA (deterministic finite automaton) M is defined by a 5-tuple:
 *      M = <Q, Σ, δ, q₀, F>
 *      - Q: finite set of states
 *      – Σ: a finite set of input symbols (alphabet)
 *      – δ: transition function δ: Q x Σ -> Q
 *      – q₀ ∈ Q: initial state
 *      – F ⊆ Q: set of accepting states
 *
 *****************************************************************************/

#include <iostream>
#include "DFA.hpp"

/**
 * @desc the main procedure takes input from file/s to initialize a DFA object.
 * the DFA object's minimize method is called and the minimized DFA/s
 * are then output to corresponding file/s and console using print methods.
 *  - DFA::minimize(), DFA::printToConsole, DFA::printToFile
 * @param argc - argument count
 * @param argv - argument vector
 * @return
 */
int main(int argc, char* argv[]) {
    // Iterate program arguments
    for (int i = 1; i < argc; i++) {
        std::cout << "======================\n";
        std::string fileName = argv[i];
        DFA dfa(fileName);  // Construct DFA from file
        dfa.minimize();     // Minimize DFA
        fileName.insert(fileName.find('.'), "-minimized");
        std::cout << "MINIMIZED ";
        dfa.printToConsole(i);  // print result to console
        dfa.printToFile(fileName);    // print result to file
    }
    std::cout << "======================\n";
    return 0;
}