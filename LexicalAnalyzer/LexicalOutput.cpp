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
    int fromState = startState;
    string tokenType;
    string lexme = "";
    string testProgram = "";
    int progSize = 0;

    while (fin >> noskipws >> ch) {
        testProgram += ch;
        progSize++;
    }
    int count = 0;
    char c = testProgram.at(count);
    cout<<testProgram<<endl;
    while (count<progSize) {
        map<int, map<string, int>>::iterator itr;
        itr = MDFA.find(fromState);
        map<string, int> m;
        m = itr->second;
        map<string, int>::iterator it;
        it = m.find(string(1, c));
        if(it ==m.end()) {
            if(c==' '||c=='\n') {
                struct token t;
                t.lexme= lexme ;
                t.TokenType = tokenType;
                lexme="";
                fromState=startState;
                allTokens.push_back(t);
                count++;
                if(count<progSize)
                    c = testProgram.at(count);
                while(c==' '||c=='\n') {
                    count++;
                    if(count<progSize) {
                        c = testProgram.at(count);
                    }else{
                        break;
                    }
                }
            }else {
                cout << "No such symbol "<<c<<" exist in the input!"<<endl;
                count++;
            }
        }else if(it->second == 0) {
            struct token t;
            t.lexme= lexme ;
            t.TokenType = tokenType;
            lexme="";
            fromState=startState;
            allTokens.push_back(t);
        }else if (isAccepted(acceptedStates,it->second)) {
            lexme += c;
            fromState = it->second;
            tokenType = getTokenType(acceptedStates, fromState);
            count++;
        }else{
            lexme+=c;
            fromState = it->second;
            count++;
        }
        if(count<progSize)
            c = testProgram.at(count);
    }
    if(lexme!=""){
        struct token t;
        t.lexme= lexme ;
        t.TokenType = tokenType;
        allTokens.push_back(t);
    }
    return allTokens;
}

string LexicalOutput :: getTokenType(vector<AcceptedState> vec, int i) {
    for(AcceptedState j : vec){
        if(j.getStateNum() == i)
            return j.getTokenType();
    }
    return "";
}

bool LexicalOutput::isAccepted(vector<AcceptedState> acceptedStates, int toState) {
    for(AcceptedState a:acceptedStates){
        if(a.getStateNum()==toState)
            return true;
    }
    return false;
}
