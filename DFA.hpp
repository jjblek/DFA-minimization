//
// Created by Justin on 5/20/2023.
//

#ifndef DFA_MINIMIZATION_454_DFA_HPP
#define DFA_MINIMIZATION_454_DFA_HPP

#include <vector>
#include <set>
#include <iostream>
#include <limits>
#include <fstream>

class DFA {
public:
    // FILE CONSTRUCTION
    explicit DFA(const std::string& fileName);

    // DIRECT CONSTRUCTION
    DFA(std::vector<int>& q,
        const std::set<char>& sigma,
        std::vector<int> f,
        std::vector<std::vector<int>> _delta
    );

    // PUBLIC METHODS
    void minimize();
    void printToConsole(int index);
    void printToFile(const std::string& fileName);

private:
    // PRIVATE METHODS
    void removeUnreachableStates();
    bool isEquivalent(int s1, int s2, const std::vector<int>& partition);

    // PRIVATE DATA
    std::vector<int> Q;                     // states
    std::set<char> Sigma;                   // alphabet
    std::vector<int> F;                     // final states
    std::vector<std::vector<int>> delta;    // transition function
};


#endif //DFA_MINIMIZATION_454_DFA_HPP
