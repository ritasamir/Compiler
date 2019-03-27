#include <iostream>
#include <vector>
#include<set>
#include <map>
#include <fstream>
#include "Minimization.h"
#include "LexicalOutput.h"
#include "Token.h"
#include "AcceptedState.h"

using namespace std;


int startState;
int nStates;// number f states
int nInputs;
set<int> acceptStates;


//int main() {
//    nInputs=2;
//    startState=1;
////    int DFA [5][2]={{2,1},
////                    {3,0},
////                    {4,3},
////                    {3,3},
////                    {2,1}};
//
//    acceptStates.insert(4);
//    acceptStates.insert(0);
//
//
//map<int,map<string,int>> DFA;
//    map<string,int> m1;
//    m1.emplace("a", 2);
//    m1.emplace("b", 1);
//    DFA.emplace(0,m1);
//    map<string,int> m2;
//    m2.emplace("a",3);
//    m2.emplace("b",0);
//    DFA.emplace(1,m2);
//    map<string,int> m3;
//    m3.emplace("a",4);
//    m3.emplace("b",3);
//    DFA.emplace(2,m3);
//    map<string,int> m4;
//    m4.emplace("a",3);
//    m4.emplace("b",3);
//    DFA.emplace(3,m4);
//    map<string,int> m5;
//    m5.emplace("a",2);
//    m5.emplace("b",1);
//    DFA.emplace(4,m5);
////    map<int,map<string,int>> :: iterator i;
////    i=DFA.find(3);
////    map<string,int> var = i-> second;
////        for(std::map<std::string,int>::iterator it1 = var.begin();
////            it1 != var.end(); ++it1){
////            std::cout << i->first <<" "<<it1->first<< " "<<it1->second  << "\n";
////        }
//    set<int> s;
//    s.insert(0);
//    s.insert(1);
//    s.insert(2);
//    s.insert(3);
//    s.insert(4);
//    vector<AcceptedState> ff;
//    AcceptedState s1;
//    s1.setStateNum(0);
//    s1.setTokenType("then");
//    ff.push_back(s1);
//    AcceptedState s2;
//    s2.setStateNum(4);
//    s2.setTokenType("else");
//    ff.push_back(s2);
//    Minimization min;
//    min.nStates=5;
//    min.acceptStates=acceptStates;
//    min.vecOfAStates=ff;
//    min.allStates=s;
//    map<int, map<string, int>>  MDFA =min.minimize(DFA);
//    vector<AcceptedState> vec = min.getFinalAcceptedStates();
//    cout<<"accepted states"<<endl;
//    for(AcceptedState i : vec){
//    cout<<i.getStateNum()<<" "<<i.getTokenType()<<endl;
//    }
//    for(map<int,map<string,int>>::iterator it = MDFA.begin();
//        it != MDFA.end(); ++it)
//    {
//        map<string,int> var = it-> second;
//        for(std::map<std::string,int>::iterator it1 = var.begin();
//            it1 != var.end(); ++it1){
//            std::cout << it->first <<" "<<it1->first<< " "<<it1->second  << "\n";
//        }
//    }
//
//
//
////    LexicalOutput lex;
////    vector<AcceptedState> acceptedStates;
////    AcceptedState t;
////    t.setStateNum(1);
////    t.setTokenType("rita");
////    acceptedStates.push_back(t);
////    vector<token> tokens;
////    string file="/home/rita/CLionProjects/LexicalAnalyzer/TestProgram.txt";
////    tokens = lex.getTokens(file,MDFA,acceptedStates,startState);
//
////    for(token i : tokens){
////        cout<<i.TokenType<<" w "<<i.lexme<<endl;
////    }
//    return 0;
//}
