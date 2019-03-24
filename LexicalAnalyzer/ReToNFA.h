//
// Created by meritv on 3/24/19.
//

#ifndef COMPILER_RETONFA_H
#define COMPILER_RETONFA_H

#include <string>
#include "NFA.h"

using namespace std;

class ReToNFA {
public:
    ReToNFA() {

    }
    vector<NFA> constructNFA();

private:
    void normalizeRe();
    NFA re_to_NFA(string re, string tokenType);
    NFA concatenate(NFA nfa1, NFA nfa2);
    NFA kleeneClosure(NFA nfa);
    NFA positiveClosure(NFA nfa);
    NFA unionNFA(vector<NFA> selections, int noOfSelections);
};


#endif //COMPILER_RETONFA_H
