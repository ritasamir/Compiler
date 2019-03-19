//
// Created by rita on 19/03/19.
//

#include <iostream>
#include "Minimization.h"


bool Minimization :: isContain(set<int> vec, int i) {
    for(int j : vec){
        if(j == i)
            return true;
    }
    return false;
}

bool Minimization :: isNotInSameSet(int i1, int i2, vector<set<int>> p) {
    for(set<int>i:p){
        for(int j:i){
            if(i1==j){
                return !isContain(i,i2);
            }
        }
    }
    return false;
}

bool Minimization :: isDistinguishable(int k, int l, int DFA[][2], vector<set<int>> p) {
    for (int i=0;i<nInputs;i++){
        if(isNotInSameSet(DFA[k][i], DFA[l][i], p))
            return true;
    }
    return false;
}


set<int> Minimization :: getRestOfSet(set<int> all, set<int> s) {
    set<int> rest;
    for(int i :all){
        if(!isContain(s,i)){
            rest.insert(i);
        }
    }
    return rest;
}

set<int> Minimization :: getNOTAcceptedStates() {
    set<int> notAccepted ;
    for(int i=0;i<nStates;i++){
        if(!isContain(acceptStates,i)){
            notAccepted.insert(i);

        }
    }

    return notAccepted;
}

void Minimization :: minimize (int DFA[][MAXSIZE]){
    //minimize the DFA
    vector<set<int>> p;
    vector<set<int>> p0;
    p0.push_back(acceptStates);
    p0.push_back(getNOTAcceptedStates());
    p=minimizeHelper(DFA,p0);
    while(!isEqual(p,p0)){
        for (set<int> j: p) {
            for (int l :j) {
                cout<<l;
            }
            cout<<"s";

        }
        cout<<endl;
        p0=p;
        p=minimizeHelper(DFA,p);
    }

}

vector<set<int>> Minimization :: minimizeHelper(int DFA[][MAXSIZE], vector<set<int>> p0) {
    vector<set<int>> p;
    for (set<int> j: p0) {
        int k = *(j.begin());
        set<int> s;
        for (int l :j) {
            if (isDistinguishable(k, l, DFA, p0)) {
                s.insert(l);
            }
        }
        if (!s.empty()) {
            p.push_back(s);
        }
        set<int> temp = getRestOfSet(j, s);
        if (!temp.empty()) {
            p.push_back(temp);
        }
    }

    return p;
}

bool Minimization::isEqual(vector<set<int>> p1, vector<set<int>> p2) {
    if(p1.size()!=p2.size())
        return false;


    return p1 == p2;
}
