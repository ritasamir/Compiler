//
// Created by meritv on 3/20/19.
//

#include <iostream>
#include <regex>
#include <ParsingCFG.h>
#include <PredictiveParser.h>
#include <LeftDerivation.h>
#include "LexicalAnalyzer.h"
#include "LexicalRulesParser.h"
#include "global.h"
#include "NFA.h"
#include "LexicalOutput.h"
#include "Token.h"
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

    if (argc < 4) {
        cout << "Wrong number of files passed\n";
    }
    ifstream file(argv[1]);

//    ifstream file("/home/rita/CLionProjects/Phase_1/lexicalRulesInput.txt");
    parseRulesFile(file);
    UnionedNFA unionedNFA = UnionedNFA();
    NFA *nfa = unionedNFA.getNFA();
    vector<AcceptedState*> acceptanceStates = unionedNFA.getAcceptanceStates();
    set<string> inputs = unionedNFA.getInputs();
    vector<string> symbols;

    symbols.assign( inputs.begin(), inputs.end() );

    vector<transition> table = nfa->getTransitions();
    DFA d = DFA(acceptanceStates,symbols,table);
    set<int> acceptedStates;
    Minimization min;
    min.allStates=d.getDFAStates();
    min.nStates=d.getNumberOfDFAStates();
    min.initialState=d.getStartState();
    for(AcceptedState a : d.getAcceptedStates())
    {
        acceptedStates.insert(a.getStateNum());
    }
    min.acceptStates = acceptedStates;
    min.vecOfAStates=d.getAcceptedStates();
    map<int, map<string, int>>  MDFA = min.minimize(d.getDfaTable());
    vector<AcceptedState> vec = min.getFinalAcceptedStates();
//    cout<<"Minimized final DFA table : "<<endl;
//    cout<<"---------------------------------"<<endl;
//    cout <<"FROM  | INPUT | TO "<<endl;
//    for(map<int,map<string,int>>::iterator it = MDFA.begin();
//            it != MDFA.end(); ++it)
//    {
//        map<string,int> var = it-> second;
//        for(map<string,int>::iterator it1 = var.begin();
//                it1 != var.end(); ++it1)
//        {
//            cout << "  "<<it->first <<"   |   "<<it1->first<< "   |   "<<it1->second  << "\n";
//        }
//    }
//
//    cout<<"----------------------------------------------------------------"<<endl;
    LexicalOutput lex;
    vector<token> tokens;
//    string f="/home/rita/CLionProjects/Phase_1/TestProgram.txt";
//    tokens = lex.getTokens(f,MDFA,vec,min.initialState);
    tokens = lex.getTokens(argv[2],MDFA,vec,min.initialState);
    cout<<"----------------------------------------------------------------"<<endl;
    for(auto &i : tokens){
        cout<<i.TokenType<<"              "<<i.lexme<<endl;
        if (i.TokenType == "assign") {i.TokenType = "=";}
    }
    ParsingCFG p ;
    vector<Production> productions;
    set<string> b= p.terminals;
    productions=p.getProductions(argv[3]);
    PredictiveParser predictiveParser =  PredictiveParser(productions, p.terminals,p.startNonTerminal);
    ParserTable *table1 = predictiveParser.table;
    table1->printTable();
    ifstream f("derivationOutput.txt", ios::out | ios::trunc);
    f.close();
    LeftDerivation l;
    l.set_terminals(predictiveParser.get_terminals());
    l.set_nonTerminals(predictiveParser.get_nonTerminals());
    l.derive(table1->getTable(),p.startNonTerminal, tokens);

}
