//
// Created by Justin on 5/20/2023.
//

#ifndef DFA_MINIMIZATION_454_DFA_HPP
#define DFA_MINIMIZATION_454_DFA_HPP

#include <vector>
#include <set>
#include <cassert>
#include <iostream>
#include <limits>
#include <fstream>

class DFA {
public:
    explicit DFA(const std::string& fileName);
    DFA(std::vector<int>& Q, const std::set<char>& Sigma, std::vector<int> f, std::vector<std::vector<int>> _delta);
    void removeUnreachableStates();
    void minimize();
    bool isEquivalent(int i, int j, const std::vector<int>& m_prime);
    void constructMinimizedDFA(std::vector<int> partition);
    void printToConsole(int index);
    void printToFile(const std::string& fileName);
private:
    std::vector<int> Q;                     // states
    std::set<char> Sigma;                   // alphabet
    std::vector<int> F;                     // final states
    std::vector<std::vector<int>> delta;    // transition function
};


#endif //DFA_MINIMIZATION_454_DFA_HPP
