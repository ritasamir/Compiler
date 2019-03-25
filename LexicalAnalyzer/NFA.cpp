//
// Created by meritv on 3/24/19.
//

#include <iostream>
#include "NFA.h"

void NFA::addStates(int noOfStates) {
    int oldsize = states.size();
    for (int i = 0; i < noOfStates; ++i) {
        states.push_back(i + oldsize);
    }
}

void NFA::addTransition(int fromState, int toState, string symbol) {
    transition trans;
    trans.fromState = fromState;
    trans.toState = toState;
    trans.trans_symbol = symbol;
    transitions.push_back(trans);
}

void NFA::setFinalState(int state) {
    finalState = state;
}

void NFA::printTransitions() {
    transition trans;
    for(int i = 0; i < transitions.size(); i++) {
        trans = transitions.at(i);
        cout << "S" << trans.fromState << " --> S" << trans.toState <<" : Symbol  "<< trans.trans_symbol << endl;
    }
    cout<< "final state : S" << getFinalState() << endl;
}

int NFA::getNumberOfStates() {
    return states.size();
}

void NFA::setTokenType(string type) {
    tokenType = type;
}

int NFA::getFinalState() {
    return finalState;
}

vector<transition> NFA::getTransitions() {
    return transitions;
}

string NFA::getTokenType() {
    return tokenType;
}
