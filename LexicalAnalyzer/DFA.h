#ifndef DFA_H
#define DFA_H
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "global.h"
#define MAXSIZE   100
#include "AcceptedState.h"
using std::string;
using namespace std;

class DFA
{
public:
    DFA(vector<AcceptedState*>finalStates, vector<string> inputs, vector<transition> nfaTable);
    map<int,map<string,int>> getDfaTable();
    int getStartState();
    int getNumberOfDFAStates();
    vector<int> getAcceptedStates();

private:
    //nfa
    vector<int> nfaStates;
    std::vector<int> nfaAcceptedStates;
    std::vector<string> symbols;
    int nfaStartState;
    std::map<int, std::map<string, std::vector<int> > > nfaTransitionsTable;
//dfa
    map<vector<int>,map<string, vector<int> > > dfaTransitionTable;
    std::vector<std::vector<int >> dfaStates;
    std::vector<int> dfaStartState;
    std::vector<std::vector<int>> dfaAcceptedStates;
//mapping dfa
    std::map<std::vector<int>,int> statesToInt;
   vector<int> acceptedStates;
    int startState;
    map<int,map<string,int>> dfaTable;
   void mappingTransitionTable(vector<transition> nfaTable);
    void generateDFAStates();
    void getEpsilonClosure(std::vector<int> &container, int state);
    void generateDFATransitionTable();
    std::vector<int> getNextState(int curState, string symbol);
    void generateAcceptStates();
    void addToStates(std::vector<int> &container);
    void generateStartState();
    void mapping();


};

#endif // DFA_H
