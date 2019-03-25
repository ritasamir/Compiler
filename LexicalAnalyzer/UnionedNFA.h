//
// Created by meritv on 3/24/19.
//

#ifndef COMPILER_UNIONEDNFA_H
#define COMPILER_UNIONEDNFA_H


#include "NFA.h"

class UnionedNFA {
public:
    UnionedNFA() {}
    NFA* getNFA();
    vector<int> getAcceptanceStates();
    int getTotalNumberOfStates();
private:
    int totalNoOfStates;
    vector<int> acceptanceStates;
};


#endif //COMPILER_UNIONEDNFA_H
