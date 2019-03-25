//
// Created by rita on 25/03/19.
//

#include "AcceptedState.h"

void AcceptedState::setStateNum(int num){
    stateNum=num;
}
void AcceptedState:: setTokenType(string s){
    tokenType=s;
}
int AcceptedState :: getStateNum(){
    return stateNum;
}
string AcceptedState :: getTokenType(){
    return tokenType;
}
