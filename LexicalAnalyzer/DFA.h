#ifndef DFA_H
#define DFA_H
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "global.h"
#include <set>
#define MAXSIZE   100
#include "AcceptedState.h"
using namespace std;

class DFA
{
public:
    DFA(vector<AcceptedState*>finalStates, vector<string> inputs, vector<transition> nfaTable);
    map<int,map<string,int>> getDfaTable();
    int getStartState();
    set<int> getDFAStates();
    int getNumberOfDFAStates();
    vector<int> getAcceptedStates();

private:
    /*NFA variables*/
    vector<int> nfaStates;
    vector<int> nfaAcceptedStates;
    vector<string> symbols;
    int nfaStartState;
    map<int, map<string, vector<int> > > nfaTransitionsTable;
    /*DFA variables*/
    map<vector<int>,map<string, vector<int> > > dfaTransitionTable;
    vector<vector<int >> dfaStates;
    vector<int> dfaStartState;
    vector<vector<int>> dfaAcceptedStates;
    /*mapping equivalent states to one state*/
    map<vector<int>,int> statesToInt;
    vector<int> acceptedStates;
    int startState;
    int numOfDFAStates;
    set<int> DFAStates;
    map<int,map<string,int>> dfaTable;
    /*Methods*/
    void mappingTransitionTable(vector<transition> nfaTable);
    void generateDFAStates();
    void getEpsilonClosure(vector<int> &container, int state);
    void generateDFATransitionTable();
    vector<int> getNextState(int curState, string symbol);
    void generateAcceptStates();
    void addToStates(vector<int> &container);
    void generateStartState();
    void mapping();
    void removeUnreachableStates();


};

#endif // DFA_H
