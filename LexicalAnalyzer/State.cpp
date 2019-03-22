#include "State.h"
using namespace std;
State::State()
{
    //ctor
}

State::~State()
{
    //dtor
}
void State::add_input(string input){
    inputs.push_back(input);
}
void State::add_output(State state){
    outputs.push_back(state);
}
void State::setAcceptanceSate(bool status){
    acceptanceState = status;
}
bool State::isAcceptanceState(){
    return acceptanceState;
}
void State::setTokenType(string type){
    tokenType = type;
}
string State::getTokenType(){
    return tokenType;
}
vector<string> State::get_input(){
    return inputs;
}
vector<State> State::get_output(){
    return outputs;
}
