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

bool Minimization :: isDistinguishable(int k, int l, map<int, map<string, int>> DFA, vector<set<int>> p) {
    map<int, map<string, int>>::iterator itr;
    itr = DFA.find(k);
    map<string, int> DFA1;
    DFA1=itr->second;
    map<int, map<string, int>>::iterator itr2;
    itr2 = DFA.find(l);
    map<string, int> DFA2;
    DFA2=itr2->second;
    map<string, int>:: iterator it2;
    map<string, int>:: iterator it1;
    it1=DFA1.begin();
    it2=DFA2.begin();
    while(it2 != DFA2.end()){
        if(isNotInSameSet(it1->second, it2->second, p))
            return true;
    it1++;
    it2++;
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
    for(int i:allStates){
        if(!isContain(acceptStates,i)){
            notAccepted.insert(i);

        }
    }

    return notAccepted;
}

map<int, map<string, int>> Minimization ::minimize(map<int, map<string, int>> DFA) {
    //minimize the DFA
    vector<set<int>> p;
    vector<set<int>> p0;
    p0=pushAcceptedStates(p0);
    p0.push_back(getNOTAcceptedStates());
    p=minimizeHelper(DFA,p0);
    while(!isEqual(p,p0)){
        p0=p;
        p=minimizeHelper(DFA,p);
    }
    pFinal=p;
    map<int, map<string, int>> MDFA = getMDFA(p, DFA);
    return MDFA;
}

vector<set<int>> Minimization :: minimizeHelper(map<int, map<string, int>> DFA, vector<set<int>> p0) {
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

map<int, map<string, int>> Minimization::getMDFA(vector<set<int>> p, map<int, map<string, int>> DFA) {
    map<int, map<string, int>> MDFA;
    for (set<int> j: p) {
       int from =*(j.begin());
       DFA= renamDFA(DFA, from, j);
//        map<int,map<string,int>>::iterator itr;
//        itr = DFA.find(from);
//        map<string, int> m;
//        m=itr->second;
    }
    for (set<int> j: p) {
        int from =*(j.begin());
        map<int,map<string,int>>::iterator itr;
        itr = DFA.find(from);
        map<string, int> m;
        m=itr->second;
        MDFA.emplace(from,m);
    }
//cout<<"numof final states "<<p.size()<<endl;
    return MDFA;
}

map<int, map<string, int>> Minimization::renamDFA(map<int, map<string, int>> DFA, int from, set<int> j) {
    map<int, map<string, int>> m;
    map<int,map<string,int>>::iterator it;
    for(it = DFA.begin();
        it != DFA.end(); ++it)
    {
        map<string,int> var = it-> second;
        for(std::map<std::string,int>::iterator it1 = var.begin();
            it1 != var.end(); ++it1){

            if(isContain(j,it1->second)) {
                it1->second = from;
                if(initialState==it1->second){
                    initialState=it1->second;
                }
            }
        }
        m.emplace(it->first,var);
    }
    return m;
}

vector<AcceptedState> Minimization::getFinalAcceptedStates() {
    vector<AcceptedState> a;
    for(AcceptedState i : vecOfAStates){
        for(set<int> j : pFinal) {
            for(int k : j) {
                if (i.getStateNum() == k) {
                    AcceptedState l;
                    l.setStateNum(*(j.begin()));
                    l.setTokenType(i.getTokenType());
                    a.push_back(l);
                }
            }
        }
    }
    return a;
}

vector<set<int>> Minimization::pushAcceptedStates(vector<set<int>> p0) {
    set<int> dummy;
    for(AcceptedState a: vecOfAStates){
        set<int> s;
        if(!isContain(dummy,a.getStateNum())){
            dummy.insert(a.getStateNum());
            s.insert(a.getStateNum());
            for (AcceptedState b: vecOfAStates) {
                if (!isContain(dummy, b.getStateNum()) && a.getTokenType() == b.getTokenType()) {
                    dummy.insert(b.getStateNum());
                    s.insert(b.getStateNum());
                }
            }
            p0.push_back(s);
        }

    }
    return p0;
}
