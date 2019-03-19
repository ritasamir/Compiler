#include <iostream>
#include <vector>
#include<set>
#include "Minimization.h"
#define MAXSIZE 2

using namespace std;


int startState;
int nStates;// number f states
int nInputs;
set<int> acceptStates;


int main() {
    nInputs=2;
    int DFA [5][2]={{2,1},
                    {3,0},
                    {4,3},
                    {3,3},
                    {2,1}};

    acceptStates.insert(4);
    acceptStates.insert(0);

    Minimization min;
    min.nStates=5;
    min.acceptStates=acceptStates;
    min.nInputs=2;
    min.minimize(DFA);

    return 0;
}
