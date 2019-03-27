//
// Created by meritv on 3/20/19.
//

#include <iostream>
#include <regex>
#include "LexicalAnalyzer.h"
#include "LexicalRulesParser.h"
#include "global.h"
#include "NFA.h"
#include "ReToNFA.h"
#include "UnionedNFA.h"
#include "DFA.h"
#include "DFA_version2.h"
#include "Minimization.h"
using namespace std;

map <string, string> regularExpressions;
vector<string> keywords;
vector<string> punctuations;

int main(int argc,char* argv[])
{
    /*
    if (argc < 2) {
        cout << "Wrong number of files passed\n";
    }
    ifstream file(argv[1]);
    */
    ifstream file("E:/study/Compiler_Phase1/LexicalAnalyzer/lexicalRulesInput.txt");
    parseRulesFile(file);
    UnionedNFA unionedNFA = UnionedNFA();
    NFA *nfa = unionedNFA.getNFA();
    vector<AcceptedState*> acceptanceStates = unionedNFA.getAcceptanceStates();
    nfa->printTransitions();
    printf("Total number of states = %d\n", unionedNFA.getTotalNumberOfStates());
    printf("Acceptance States:\n");
    for (int i = 0; i < acceptanceStates.size(); ++i)
    {
        printf("S%d: %s\n", acceptanceStates[i]->getStateNum(), acceptanceStates[i]->getTokenType().c_str());
    }
    cout<<"------------------------------------------------------------"<<endl;
    set<string> inputs = unionedNFA.getInputs();
    vector<string> symbols;

    symbols.assign( inputs.begin(), inputs.end() );

    vector<transition> table = nfa->getTransitions();
    DFA_version2 d = DFA_version2(acceptanceStates,symbols,table);

//    DFA d = DFA(acceptanceStates,symbols,table);

    cout<<"------------------------------------------------------------"<<endl;

    set<int> acceptedStates;
    Minimization min;
    min.allStates=d.getDFAStates();
    min.nStates=d.getNumberOfDFAStates();
    for(AcceptedState a : d.getAcceptedStates())
    {
        acceptedStates.insert(a.getStateNum());
    }
    min.acceptStates = acceptedStates;
    min.vecOfAStates=d.getAcceptedStates();
    map<int, map<string, int>>  MDFA = min.minimize(d.getDfaTable());
    vector<AcceptedState> vec = min.getFinalAcceptedStates();
    cout<<"Final accepted states"<<endl;
    for(AcceptedState i : vec)
    {
        cout<<i.getStateNum()<<" "<<i.getTokenType()<<endl;
    }
    cout<<"Minimized final DFA table"<<endl;
    for(map<int,map<string,int>>::iterator it = MDFA.begin();
            it != MDFA.end(); ++it)
    {
        map<string,int> var = it-> second;
        for(std::map<std::string,int>::iterator it1 = var.begin();
                it1 != var.end(); ++it1)
        {
            std::cout << it->first <<" "<<it1->first<< " "<<it1->second  << "\n";
        }
    }

}
