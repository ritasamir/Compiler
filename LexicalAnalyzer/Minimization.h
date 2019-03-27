//
// Created by rita on 19/03/19.
//

#ifndef LEXICALANALYZER_MINIMIZATION_H
#define LEXICALANALYZER_MINIMIZATION_H
#include <vector>
#include<set>
#include <map>
#include "AcceptedState.h"

using namespace std;

class Minimization {
public:
    int nStates;    // number of states
    set<int> acceptStates;
    set<int> allStates;
    vector<AcceptedState> vecOfAStates;
    map<int, map<string, int>> minimize(map<int, map<string, int>> DFA);

    vector<AcceptedState> getFinalAcceptedStates();

private:
    vector<set<int>> pFinal;

    bool isContain(set<int> vector, int i);

    set<int> getNOTAcceptedStates();

    set<int> getRestOfSet(set<int> all, set<int> s);

    bool isDistinguishable(int k, int l, map<int, map<string, int>> DFA, vector<set<int>> vector1);

    bool isNotInSameSet(int i1, int i2, vector<set<int>> vector1);

    vector<set<int>> minimizeHelper(map<int, map<string, int>> DFA, vector<set<int>> p0);

    bool isEqual(vector<set<int>> p1, vector<set<int>> p2);

    map<int, map<string, int>> getMDFA(vector<set<int>> p, map<int, map<string, int>> map1);

    map<int, map<string, int>> renamDFA(map<int, map<string, int>> map, int from, set<int> set1);

    vector<set<int>> pushAcceptedStates(vector<set<int>> p0);
};


#endif //LEXICALANALYZER_MINIMIZATION_H
