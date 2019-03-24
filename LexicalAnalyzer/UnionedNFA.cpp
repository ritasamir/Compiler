//
// Created by meritv on 3/24/19.
//

#include "UnionedNFA.h"
#include "ReToNFA.h"

NFA UnionedNFA::getNFA() {
    ReToNFA reToNFA;
    vector<NFA> nfaForEachRe;
    nfaForEachRe = reToNFA.constructNFA();
    int totalNoOfStates = 1;
    for (int i = 0; i < nfaForEachRe.size(); ++i) {
        totalNoOfStates += nfaForEachRe.at(i).getNumberOfStates();
    }
    NFA med;
    NFA result;
    result.addStates(totalNoOfStates);
    result.setFinalState(totalNoOfStates - 1);
    int tracker = 1;
    transition trans;
    for(int i = 0; i < nfaForEachRe.size(); i++) {
        result.addTransition(0, tracker, "\\L");
        med = nfaForEachRe.at(i);
        for(int j = 0; j < med.getTransitions.size(); j++) {
            trans = med.getTransitions.at(j);
            result.addTransition(trans.fromState + tracker, trans.toState + tracker, trans.trans_symbol);
        }
        tracker += med.getNumberOfStates();
        acceptanceStates.push_back(tracker - 1);
    }
    return result;
}

vector<int> UnionedNFA::getAcceptanceStates() {
    return acceptanceStates;
}
