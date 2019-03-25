//
// Created by meritv on 3/24/19.
//

#include "UnionedNFA.h"
#include "ReToNFA.h"
#include "AcceptedState.h"

NFA* UnionedNFA::getNFA() {
    ReToNFA reToNFA;
    vector<NFA*> nfaForEachRe;
    nfaForEachRe = reToNFA.constructNFA();
    totalNoOfStates = 1;
    for (int i = 0; i < nfaForEachRe.size(); ++i) {
        totalNoOfStates += nfaForEachRe.at(i)->getNumberOfStates();
    }
    NFA* med;
    NFA* result = new NFA();
    result->addStates(totalNoOfStates);
    int tracker = 1;
    transition trans;
    inputs.insert("\\L");
    for(int i = 0; i < nfaForEachRe.size(); i++) {
        AcceptedState* acceptedState = new AcceptedState();
        result->addTransition(0, tracker, "\\L");
        med = nfaForEachRe.at(i);
        for(int j = 0; j < med->getTransitions().size(); j++) {
            trans = med->getTransitions().at(j);
            result->addTransition(trans.fromState + tracker, trans.toState + tracker, trans.trans_symbol);
            inputs.insert(trans.trans_symbol);
        }
        tracker += med->getNumberOfStates();
        acceptedState->setTokenType(nfaForEachRe.at(i)->getTokenType());
        acceptedState->setStateNum(tracker - 1);
        acceptanceStates.push_back(acceptedState);
    }
    return result;
}

vector<AcceptedState*> UnionedNFA::getAcceptanceStates() {
    return acceptanceStates;
}

int UnionedNFA::getTotalNumberOfStates() {
    return totalNoOfStates;
}

set<string> UnionedNFA::getInputs() {
    return inputs;
}
