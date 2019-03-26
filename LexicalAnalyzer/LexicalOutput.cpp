//
// Created by rita on 25/03/19.
//

#include "LexicalOutput.h"

using namespace std;

vector <token> LexicalOutput::getTokens(string file, map<int, map<string, int>> MDFA,vector<AcceptedState> acceptedStates
,int startState) {
    vector<token> allTokens;
    char ch;
    fstream fin(file, fstream::in);
    int fromState=startState;
    string tokenType;
    string lexme="";
    string testProgram="";
    int progSize=0;
    while(fin >> noskipws >> ch){
        testProgram+=ch;
        progSize++;
    }
    int count=0;
    char c = testProgram.at(count);
    while (count<progSize) {
        map<int, map<string, int>>::iterator itr;
        itr = MDFA.find(fromState);
        map<string, int> m;
        m = itr->second;
        map<string, int>::iterator it;
        it = m.find(string(1, c));
        if(it ==m.end()){
            struct token t;
            t.lexme= lexme ;
            t.TokenType = tokenType;
            lexme="";
            fromState=startState;
            allTokens.push_back(t);
            while(c == ' '){
                count++;
                if(count<progSize)
                    c = testProgram.at(count);
            }
        }else {
            lexme += c;
            fromState = it->second;
            string temp = isContain(acceptedStates, fromState);
            if (temp != "") {
                tokenType = temp;
                cout<<temp<<" "<<lexme<<endl;
            }

            count++;
            if(count<progSize)
                c = testProgram.at(count);
        }
    }
    return allTokens;
}

string LexicalOutput :: isContain(vector<AcceptedState> vec, int i) {
    for(AcceptedState j : vec){
        if(j.getStateNum() == i)
            return j.getTokenType();
    }
    return "";
}
