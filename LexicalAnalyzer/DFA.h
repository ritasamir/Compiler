#ifndef DFA_H
#define DFA_H
#include <vector>
#include <map>
#include<string>
#include <iostream>

using std::string;

class DFA
{
    public:
        DFA();
        void convertToDFA();
   std::vector<string> symbols;
	std::vector<std::vector<string>> dfaStates;

	std::map<std::vector<std::string>, std::map<std::string, std::vector<std::string> > > dfaTransitionTable;
     std::vector<string> dfaStartState;
	std::vector<std::vector<string>> dfaAcceptedStates;

	void generateDFAStates();
	void getEpsilonClosure(std::vector<string> &container, string state);
	void generateDFATransitionTable();
	std::vector<string> getNextState(string curState, string symbol);
	void generateAcceptStates();
	void addToStates(std::vector<string> &container);
	void generateStartState();

    //test
    void printDFA();
	void test();
};

#endif // DFA_H
