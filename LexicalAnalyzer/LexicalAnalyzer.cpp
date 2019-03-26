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
    ifstream file("/home/rita/CLionProjects/LexicalAnalyzer/lexicalRulesInput.txt");
    parseRulesFile(file);
    UnionedNFA unionedNFA = UnionedNFA();
    NFA *nfa = unionedNFA.getNFA();
    vector<AcceptedState*> acceptanceStates = unionedNFA.getAcceptanceStates();
   // nfa->printTransitions();
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

    DFA d = DFA(acceptanceStates,symbols,table);

    cout<<"------------------------------------------------------------"<<endl;

    set<int> acceptedStates;
    Minimization min;
    min.allStates=d.getDFAStates();
    min.nStates=d.getNumberOfDFAStates();
    for(int a : d.getAcceptedStates()){
        acceptedStates.insert(a);
    }
    min.acceptStates = acceptedStates;
//    vector<AcceptedState> ff;
//    AcceptedState s;
//    s.setStateNum(4);
//    s.setTokenType("if");
//    ff.push_back(s);
//    AcceptedState s1;
//    s1.setStateNum(9);
//    s1.setTokenType("then");
//    ff.push_back(s1);
//    AcceptedState s2;
//    s2.setStateNum(14);
//    s2.setTokenType("else");
//    ff.push_back(s2);

    map<int, map<string, int>>  MDFA =min.minimize(d.getDfaTable());

//    vector<AcceptedState> vec = min.getFinalAcceptedStates(ff);
//    cout<<"accepted states"<<endl;
//    for(AcceptedState i : vec){
//    cout<<i.getStateNum()<<" "<<i.getTokenType()<<endl;
//    }

    for(map<int,map<string,int>>::iterator it = MDFA.begin();
        it != MDFA.end(); ++it)
    {
        map<string,int> var = it-> second;
        for(std::map<std::string,int>::iterator it1 = var.begin();
            it1 != var.end(); ++it1){
            std::cout << it->first <<" "<<it1->first<< " "<<it1->second  << "\n";
        }
    }
}
