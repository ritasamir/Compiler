#include "States.h"
using namespace std;
States::States()
{
    //ctor
}

States::~States()
{
    //dtor
}
void States::add_input(string input){
    inputs.push_back(input);
}
void States::add_output(States state){
    outputs.push_back(state);
}
void States::setAcceptanceSate(bool status){
    acceptanceState = status;
}
bool States::isAcceptanceState(){
    return acceptanceState;
}
void States::setTokenType(string type){
    tokenType = type;
}
string States::getTokenType(){
    return tokenType;
}
vector<string> States::get_input(){
    return inputs;
}
vector<States> States::get_output(){
    return outputs;
}
