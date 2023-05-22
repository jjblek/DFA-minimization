#include "DFA.hpp"

/**
 * Construct a DFA object directly.
 * With the following parameters:
 * @param q      - states
 * @param sigma  - alphabet
 * @param f      - accepting states
 * @param _delta - transition function
 */
DFA::DFA(std::vector<int>& q,
    const std::set<char>& sigma,
    std::vector<int> f,
    std::vector<std::vector<int>> _delta) {
    Q = std::move(q),
    Sigma = sigma,
    F = std::move(f),
    delta = std::move(_delta);
}



/**
 * Construct a DFA object from file.
 * Using the following format:
 *  6        (number of states)
 *  2        (alphabet size)
 *  1 2 4    (accepting states)
 *  3 1      (transitions on state 0)
 *  2 5      (transitions on state 1)
 *  2 5      (transitions on state 2)
 *  0 4      (transitions on state 3)
 *  2 5      (transitions on state 4)
 *  5 5      (transitions on state 5)
 * With the following parameter:
 * @param fileName - name of the file
 */
DFA::DFA(const std::string& fileName) {
    std::ifstream file(fileName);
    int num_states;
    int alphabet_size;
    file >> num_states;
    file >> alphabet_size;

    delta.resize(num_states, std::vector<int>(alphabet_size));

    if (!file) {
        std::cerr << "error opening input file\n";
        exit(1);
    }

    for (int i = 0; i < num_states; i++)
        Q.push_back(i);

    for (int i = 0; i < alphabet_size; i++)
        Sigma.insert(i + 48);

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string line;
    std::getline(file, line);

    for (char ch : line)
        if (ch != ' ') F.push_back(ch - 48);


    for (auto &state : delta) {
        for (auto &nextState : state)
            file >> nextState;
    }
}



/**
 * Function to minimize a DFA object.
 * Performing the following steps:
 * 1. Remove unreachable states
 * 2. Apply the state minimization algorithm iteratively
 * 3. Construct the minimized DFA
 */
void DFA::minimize() {
    // Step 1. Remove unreachable states
    removeUnreachableStates();

    // Step 2: Apply state minimization algorithm iteratively
    // Initialize the partition
    std::vector<int> partition(Q.size(), -1);
    for (auto &i: F) partition[i] = *F.begin();

    // Iterate until partition is the same as newPartition
    for(;;) {
        // Create a new partition for the current iteration
        std::vector<int> newPartition(Q.size(), -1);

        // Initialize s1 to 0 and iterate through the states
        for (int s1 = 0; s1 != Q.size();) {

            newPartition[s1] = s1; // Assign s1 in the new partition

            int next = int(Q.size()); // Initialize next as max state index

            // Compare s1 with all states that have not been assigned
            for (int s2 = s1 + 1; s2 < Q.size(); s2++) {

                // If s2 is already assigned, skip to next iteration
                if (newPartition[s2] != -1) continue;

                // Else, if s1 and s2 are equivalent, assign s2 to s1 in new partition
                else if (isEquivalent(s1, s2, partition)) newPartition[s2] = s1;

                // Else, if next is still max state index, update to s2
                else if (next == Q.size()) next = s2;
            }
            // Update s1 to the state index for the next iteration
            s1 = next;
        }

        // If the partition has changed, update it
        if (partition != newPartition) partition = newPartition;

        // Else terminate when replacement does not change current partition
        else break;

    }

    // Step 3: Construct the minimized DFA using the final partition
    constructMinimizedDFA(partition);
}



/**
 * Helper Function to remove unreachable states from the DFA.
 * Unreachable states are states not reachable from the initial state
 */
void DFA::removeUnreachableStates() {
    // Set of reachable states starting with the initial state
    std::set<int> reachable_states = {0};
    // Set of new states discovered in each iteration
    std::set<int> new_states = {0};

    // Find new reachable states iteratively
    do {
        // Temporary set to store states reachable from new_states
        std::set<int> temp;
        for (auto q : new_states) {
            // Follow transitions for each input symbol
            for (auto i = 0; i < Sigma.size(); ++i) {
                int p = delta[q][i];
                temp.insert(p);
            }
        }
        // Clear new_states and add only previously unmarked states
        new_states.clear();
        for (auto p : temp) {
            if (reachable_states.find(p) == reachable_states.end()) {
                new_states.insert(p);
            }
        }
        // Add new_states to reachable_states
        reachable_states.insert(new_states.begin(), new_states.end());
    } while (!new_states.empty());

    // Set of unreachable states
    std::set<int> unreachable_states;
    for (auto & q : Q) {
        if (reachable_states.find(q) == reachable_states.end()) {
            unreachable_states.insert(q);
        }
    }

    // Clear original set of states and update it with reachable states
    Q.clear();
    std::copy(reachable_states.begin(), reachable_states.end(),
              std::inserter( Q, Q.begin() ) );

    // Remove rows from the transition table corresponding to unreachable states
    for (auto i = unreachable_states.rbegin(); i != unreachable_states.rend() ; ++i) {
        delta.erase(delta.begin()+*i);
    }

    // Update accepting states and transition table to reflect new state indices
    for (int state = 0; state < Q.size(); ++state) {
        if (Q[state] != state) {
            // Update accepting states
            for (int & accepting_state : F) {
                if (accepting_state == Q[state]) accepting_state = state;
            }
            // Update transition table
            for (auto & transitions : delta) {
                for (int & transition : transitions) {
                    if (transition == Q[state]) transition = state;
                }
            }
            // Update state index in Q
            Q[state] = state;
        }
    }
}



/**
 * Helper Function to determine whether two states of a DFA are equivalent.
 *  If two states have transitions to the same states
 *  for all possible input symbols, they are considered equivalent.
 * @param s1        - state 1
 * @param s2        - state 2
 * @param partition - current partition
 * @return true if equivalent
 */
bool DFA::isEquivalent(int s1, int s2, const std::vector<int>& partition) {
    for (int transition = 0; transition < Sigma.size(); transition++) {
        int delta_s1 = delta[s1][transition];
        int delta_s2 = delta[s2][transition];
        if (partition[delta_s1] != partition[delta_s2])
            return false;
    }
    return true;
}



/**
 * Helper function to construct a minimized DFA object.
 * @param partition - minimized partition
 */
void DFA::constructMinimizedDFA(std::vector<int> partition) {
    // Copy minimization to new set
    std::set<int> newStates(partition.begin(), partition.end());

    // Initialize new delta function
    std::vector<std::vector<int> > newDelta(newStates.size(), std::vector<int>(Sigma.size(), -1));
    for (int i = 0; i < partition.size(); i++) {
        // Rename state if needed
        int state = std::distance(newStates.begin(), newStates.find(partition[i]));
        for (int j = 0; j < Sigma.size(); j++) {
            // Rename next state
            int nextState = std::distance(newStates.begin(), newStates.find(partition[delta[i][j]]));

            newDelta[state][j] = nextState;
        }
    }
    // New states
    Q.clear();
    std::copy(newStates.begin(), newStates.end(),
              std::inserter( Q, Q.begin()));

    // New accepting states
    std::set<int> newAcceptingStates;
    for (const auto & state : F)
        newAcceptingStates.insert(partition[state]);
    F.clear();
    std::copy(newAcceptingStates.begin(), newAcceptingStates.end(),
              std::inserter( F, F.begin()));

    // Copy new delta
    delta = newDelta;

    // Change state names
    for (int state = 0; state < Q.size(); ++state) {
        if (Q[state] != state) {
            for (int & accepting_state : F)
                if (accepting_state == Q[state])
                    accepting_state = state;
        }
    }
}



/**
 * Function to print a DFA object to file
 * @param fileName - target file
 */
void DFA::printToFile(const std::string& fileName) {
    std::ofstream file(fileName);

    if (!file) {
        std::cerr << "error opening output file\n";
        exit(1);
    }

    file << Q.size() << '\n' << Sigma.size() << '\n';

    for (const auto &state : F) {
        file << state << ' ';
    }
    file << '\n';

    for (const auto &state : delta) {
        for (const auto & nextState : state) {
            file << nextState << ' ';
        }
        file << '\n';
    }

    file.close();
}



/**
 * Function to print a DFA object to console
 * @param index - DFA index
 */
void DFA::printToConsole(int index) {
    std::cout << "DFA " << index << std::endl;
    std::cout << "Total states  -  " << Q.size() << '\n' << "Alphabet Size -  " << Sigma.size() << '\n';
    std::cout << "Final States  -  ";
    for (const auto &state : F) {
        std::cout << state << ' ';
    }
    std::cout << '\n';
    for (int state = 0; state < delta.size(); ++state) {
        std::cout << "Transition " << state << "  -  ";
        for (int transition : delta[state]) {
            std::cout << transition << ' ';
        }
        std::cout << '\n';
    }
}
