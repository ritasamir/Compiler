#ifndef DFA_H
#define DFA_H
#include <vector>
#include <map>
#include<string>
#include <iostream>
#define MAXSIZE   100
using std::string;

class DFA
{
public:
    DFA();
    std::map<std::vector<int>,int> statesToInt;
    std::vector<int> acceptedStates;
    int startState;
    std::map<int,std::map<std::string,int>> dfaTable;
    //test
    void printDFA();
    void test();


private:
    std::vector<string> symbols;
    std::vector<std::vector<int >> dfaStates;
    std::map<std::vector<int>, std::map<std::string, std::vector<int> > > dfaTransitionTable;
    std::vector<int> dfaStartState;
    std::vector<std::vector<int>> dfaAcceptedStates;

    void convertToDFA();
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
