# CS 454 - Project 2

<sub>Zach Gassner, Justin Blechel</sub>

---
Implement an algorithm to minimize a DFA.
*  The standard algorithm that runs in O(mn<sup>2</sup>) time can be improved. One such faster algorithm is provided in an earlier version of our text book.
## Description

---
A DFA (deterministic finite automaton) M is defined by a 5-tuple:
- M = <Q, Σ, δ, q₀, F>
    * Q: finite set of states
    * Σ: a finite set of input symbols (alphabet)
    * δ: transition function δ: Q x Σ -> Q
    * q₀ ∈ Q: initial state
    * F ⊆ Q: set of accepting states

Minimizing means reducing the number of states in a DFA.
We must detect the states whose presence or absence in the DFA does not affect the language accepted by the DFA. 
These states can be eliminated from the DFA.

The program consists of a singular class 'DFA' used to represent a deterministic finite automata. 
A DFA object is initialized with a text file that is passed to the main procedure as a program argument.
The class includes a method DFA::Minimize() which is used to minimize an input DFA.


## Getting Started

---
### Input DFA
A DFA is initialized using the data from a text file. 

For example, a DFA **M** = <Q, Σ, δ, q₀, F> where,
* Q:= 6
* Σ:= 2
* δ:= {{3, 1}, {2, 5}, {2, 5}, {0, 4}, {2, 5}, {5, 5}}
* q₀:= 0
* F:= {1, 2, 4}

should be entered into a .txt file as such:
```
6
2
1 2 4
3 1
2 5
2 5
0 4
2 5
5 5
```
### Execute the program
Change to the working directory and make the program. Execute the program with a text file (representing a DFA) as the program argument. More than one DFA can be passed as program arguments.
#### 1 DFA
```
cd <working directory>
make
./minimize dfa.txt
```
#### 2 DFAs
```
cd <working directory>
make
./minimize dfa1.txt dfa2.txt
```
