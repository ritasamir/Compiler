//
// Created by rita on 25/03/19.
//

#ifndef LEXICALANALYZER_LEXICALOUTPUT_H
#define LEXICALANALYZER_LEXICALOUTPUT_H
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include "Token.h"
#include "AcceptedState.h"
using namespace std;

class LexicalOutput {

public:
    vector <token> getTokens(string line, map<int, map<string, int>> MDFA,vector<AcceptedState> acceptedStates
            ,int startState);

private:
    string getTokenType(vector<AcceptedState> vec, int i);

    bool isAccepted(vector<AcceptedState> acceptedStates, int toState);
};


#endif //LEXICALANALYZER_LEXICALOUTPUT_H
