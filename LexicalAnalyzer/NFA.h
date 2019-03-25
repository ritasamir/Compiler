//
// Created by meritv on 3/24/19.
//

#ifndef COMPILER_NFA_H
#define COMPILER_NFA_H


#include "global.h"

class NFA {
public:
    NFA() {}
    vector<transition> getTransitions();
    int getFinalState();
    vector<int> getStates;
    void addStates(int noOfStates);
    void addTransition(int fromState, int toState, string symbol);
    void setFinalState(int state);
    void printTransitions();
    int getNumberOfStates();
    void setTokenType(string type);
    string getTokenType();
private:
    vector<transition> transitions;
    int finalState;
    vector<int> states;
    string tokenType;
};


#endif //COMPILER_NFA_H
