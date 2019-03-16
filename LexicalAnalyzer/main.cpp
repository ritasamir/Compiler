#include <iostream>
#include <vector>
#include "Elem.h"
#define MAXSIZE 10
using namespace std;


Elem transitionTable [MAXSIZE][MAXSIZE];
int startState;
vector<int> acceptStates;

int main() {
    return 0;
}
void getNFA (string rulesFile){
    //regular expression to NFA
}

void getDFA (Elem NFA[][MAXSIZE]){
    //translate the NFA to DFA
}
void minimize (Elem transitionTable[][MAXSIZE]){
    //minimize the DFA
}
