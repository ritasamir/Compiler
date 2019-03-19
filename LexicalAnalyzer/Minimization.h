//
// Created by rita on 19/03/19.
//

#ifndef LEXICALANALYZER_MINIMIZATION_H
#define LEXICALANALYZER_MINIMIZATION_H
#include <vector>
#include<set>
#define MAXSIZE 2
using namespace std;

class Minimization {
public:
    int nStates;    // number of states
    int nInputs;    // number of different inputs
    set<int> acceptStates;
    void minimize (int DFA[][MAXSIZE]);

private:

    bool isContain(set<int> vector, int i);

    set<int> getNOTAcceptedStates();

    set<int> getRestOfSet(set<int> all, set<int> s);

    bool isDistinguishable(int k, int l, int DFA[][2], vector<set<int>> vector1);

    bool isNotInSameSet(int i1, int i2, vector<set<int>> vector1);

    vector<set<int>> minimizeHelper(int DFA[][MAXSIZE], vector<set<int>> p0);

    bool isEqual(vector<set<int>> p1, vector<set<int>> p2);
};


#endif //LEXICALANALYZER_MINIMIZATION_H
