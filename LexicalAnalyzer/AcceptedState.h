//
// Created by rita on 25/03/19.
//

#ifndef LEXICALANALYZER_ACCEPTEDSTATE_H
#define LEXICALANALYZER_ACCEPTEDSTATE_H

#include <string>
using namespace std;

class AcceptedState {
private:
    int stateNum;
    string tokenType;
public:
    void setStateNum(int num);
    void setTokenType(string s);
    int getStateNum();
    string getTokenType();

};


#endif //LEXICALANALYZER_ACCEPTEDSTATE_H
