#ifndef DFA_VERSION2_H
#define DFA_VERSION2_H
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "global.h"
#include <set>
#include "AcceptedState.h"
using namespace std;
class DFA_version2
{
public:
    DFA_version2(vector<AcceptedState*>finalStates, vector<string> inputs, vector<transition> nfaTable);
    map<int,map<string,int>> getDfaTable();
    int getStartState();
    set<int> getDFAStates();
    int getNumberOfDFAStates();
    vector<AcceptedState> getAcceptedStates();

private:
    vector<AcceptedState*> nfaAcceptedStates;
    vector<string> symbols;
    vector<int> nfaStartState;
    map<int, map<string, vector<int>>> nfaTransitionsTable;

    map<vector<int>, bool> checked_states;
    map<vector<int>,int> statesToInt;
    map<vector<int>,map<string, vector<int>>> dfaTransitionTable;
    vector<pair<vector<int>,string>> dfaAcceptedStates;

    vector<int> dfaStates;
    vector<int> dfaStartState;
    int startState;
    vector<AcceptedState> acceptedStates;
    map<int,map<string,int>> dfaTable;

    void mappingTransitionTable(vector<transition> nfaTable);
    vector<int> getNextState(int curState, string symbol);
    void generateDFA();
    bool isTransitionExist(int state, string ch);
    void generateAcceptStates();
    vector<int> getEpsilonClosure(vector<int> state);
    void convertToDFA();
};

#endif // DFA_VERSION2_H
