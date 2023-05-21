#include "DFA.hpp"

DFA::DFA(std::vector<int>& q,
         const std::set<char>& sigma,
         std::vector<int> f,
         std::vector<std::vector<int>> _delta) {
    Q = std::move(q),
            std::copy(
                    sigma.begin(), sigma.end(),
                    std::inserter( Sigma, Sigma.begin() ) );

    F = std::move(f),
            delta = std::move(_delta);

}

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

    for (char ch : line) {
        if (ch != ' ') {
            F.push_back(ch - 48);
        }
    }

    for (auto &state : delta) {
        for (auto &nextState : state) {
            file >> nextState;
        }
    }
}

bool DFA::isEquivalent(int i, int j, const std::vector<int>& m_prime) {
    for (int transition_index = 0; transition_index < Sigma.size(); transition_index++) {
        int destination_i = delta[i][transition_index];
        int destination_j = delta[j][transition_index];
        if (m_prime[destination_i] != m_prime[destination_j])
            return false;
    }
    return true;
}

void DFA::Minimize() {
    const int states = int(Q.size());

    std::vector<int> partition(states, -1);

    // start off by separating the accepting from the rejecting states,
    int rejecting = -1;

    for (auto &i: F) partition[i] = *F.begin();

    for (int i = 0; i < states; i++) {
        if (partition[i] < 0) {
            if (rejecting < 0) rejecting = i;
            partition[i] = rejecting;
        }
    }
    // At each step, replace the current partition
    // with the coarsest common refinement of s + 1 partitions,
    // one of which is the current one and the rest of which
    // are the preimages of the current partition under
    // the transition functions for each of the input symbols.
    while (true) {
        std::vector<int> preimage(states, -1);
        int s = 0;
        while (s < states) {
            preimage[s] = s;
            int next = states;
            // replace the current partition with the coarsest common refinement of s + 1 partitions
            for (int i = s + 1; i < states; i++) {
                if (preimage[i] >= 0) continue;  // skip if i is already replaced
                if (partition[s] == partition[i] && isEquivalent(s, i, partition)) {
                    preimage[i] = s;   // merge s & i
                }
                else if (next == states) next = i;  // keep the first unmerged node
            }
            s = next;
        }
        // terminate when replacement does not change the current partition
        if (partition == preimage) break;
        partition = preimage;
    }

    // copy minimization to new set
    std::set<int> newStates(partition.begin(), partition.end());

    // initialize new delta function
    std::vector<std::vector<int> > newDelta(newStates.size(), std::vector<int>(Sigma.size(), -1));
    for (int i = 0; i < partition.size(); i++) {
        // renaming state if needed
        int state = std::distance(newStates.begin(), newStates.find(partition[i]));
        for (int j = 0; j < Sigma.size(); j++) {
            // renaming next state
            int nextState = std::distance(newStates.begin(), newStates.find(partition[delta[i][j]]));

            newDelta[state][j] = nextState;
        }
    }

    // new accepting states
    std::set<int> newAcceptingStates;
    for (const auto & state : F) {
        newAcceptingStates.insert(partition[state]);
    }
    Q.clear();
    std::copy(
            newStates.begin(), newStates.end(),
            std::inserter( Q, Q.begin() ) );

    F.clear();
    std::copy(newAcceptingStates.begin(), newAcceptingStates.end(),
              std::inserter( F, F.begin() ) );
    delta = newDelta;

    for (int state = 0; state < Q.size(); ++state) {
        if (Q[state] != state) {
            for (int & accepting_state : F) {
                if (accepting_state == Q[state]) accepting_state = state;
            }
        }
    }
}

// Function to remove unreachable states
void DFA::removeUnreachableStates() {
    std::set<int> reachable_states = {0};
    std::set<int> new_states = {0};

    do {
        std::set<int> temp;
        for (auto q : new_states) {
            for (auto i = 0; i < Sigma.size(); ++i) {
                int p = delta[q][i];
                temp.insert(p);
            }
        }
        new_states.clear();
        for (auto p : temp) {
            if (reachable_states.find(p) == reachable_states.end()) {
                new_states.insert(p);
            }
        }
        reachable_states.insert(new_states.begin(), new_states.end());
    } while (!new_states.empty());

    std::set<int> unreachable_states;
    for (auto & q : Q) {
        if (reachable_states.find(q) == reachable_states.end()) {
            unreachable_states.insert(q);
        }
    }

    Q.clear();
    std::copy(reachable_states.begin(), reachable_states.end(),
              std::inserter( Q, Q.begin() ) );

    for (auto i = unreachable_states.rbegin(); i != unreachable_states.rend() ; ++i) {
        delta.erase(delta.begin()+*i);
    }

    for (int state = 0; state < Q.size(); ++state) {
        if (Q[state] != state) {
            for (int & accepting_state : F) {
                if (accepting_state == Q[state]) accepting_state = state;
            }
            for (auto & transitions : delta) {
                for (int & transition : transitions) {
                    if (transition == Q[state]) transition = state;
                }
            }
            Q[state] = state;
        }
    }
}

/**
 * Function to print a DFA object to file
 * @param fileName target file
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
 * @param name oif the DFA
 */
void DFA::printToConsole(int index) {
    std::cout << "DFA " << index << ": " << std::endl;
    std::cout << Q.size() << '\n' << Sigma.size() << '\n';

    for (const auto &state : F) {
        std::cout << state << ' ';
    }
    std::cout << '\n';

    for (const auto &state : delta) {
        for (const auto & nextState : state) {
            std::cout << nextState << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}
