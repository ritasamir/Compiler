#ifndef DFA_H
#define DFA_H
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "global.h"
#include <set>
#include "AcceptedState.h"
using namespace std;
class DFA
{
public:
    /*Constructor*/
    DFA(vector<AcceptedState*>finalStates, vector<string> inputs, vector<transition> nfaTable);
    /*method to return DFA Transition Table*/
    map<int,map<string,int>> getDfaTable();
    int getStartState();
    /*method to return all DFA states without any duplicates*/
    set<int> getDFAStates();
    int getNumberOfDFAStates();
    /*method to return all DFA Accepted/Final states without any duplicates*/
    vector<AcceptedState> getAcceptedStates();

private:
    /*All Data needed from NFA*/
    vector<AcceptedState*> nfaAcceptedStates;
    vector<string> symbols;
    vector<int> nfaStartState;
    map<int, map<string, vector<int>>> nfaTransitionsTable;
    /*DFA Variables*/
    map<vector<int>, bool> checked_states;
    map<vector<int>,int> statesToInt;
    map<vector<int>,map<string, vector<int>>> dfaTransitionTable;
    vector<pair<vector<int>,string>> dfaAcceptedStates;
    vector<int> dfaStartState;
    /*DFA Variables after mapping each equivalent states to one state*/
    vector<int> dfaStates;
    int startState;
    vector<AcceptedState> acceptedStates;
    map<int,map<string,int>> dfaTable;
    /*DFA Methods*/
    void mappingTransitionTable(vector<transition> nfaTable);
    vector<int> getNextState(int curState, string symbol);
    void generateDFA();
    bool isTransitionExist(int state, string ch);
    void generateAcceptStates();
    vector<int> getEpsilonClosure(vector<int> state);
    void convertToDFA();
};

#endif // DFA_VERSION2_H
