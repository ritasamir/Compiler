//
// Created by meritv on 3/24/19.
//

#ifndef COMPILER_UNIONEDNFA_H
#define COMPILER_UNIONEDNFA_H


#include <set>
#include "NFA.h"
#include "AcceptedState.h"

class UnionedNFA {
public:
    UnionedNFA() {}
    NFA* getNFA();
    vector<AcceptedState*> getAcceptanceStates();
    int getTotalNumberOfStates();
    set<string> getInputs();
private:
    int totalNoOfStates;
    vector<AcceptedState*> acceptanceStates;
    set<string> inputs;
};


#endif //COMPILER_UNIONEDNFA_H
