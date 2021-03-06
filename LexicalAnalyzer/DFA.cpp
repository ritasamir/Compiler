#include "DFA.h"
#include <queue>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <algorithm>
#include<string.h>
#include <sstream>
#include "AcceptedState.h"
using namespace std;
/*Constructor*/
DFA::DFA(vector<AcceptedState*> finalStates, vector<string> inputs,vector<transition> nfaTable)
{
    nfaAcceptedStates = finalStates;
    symbols = inputs;
    symbols.erase(std::remove(symbols.begin(), symbols.end(), "\\L"), symbols.end());
    nfaStartState.push_back(0);
    mappingTransitionTable(nfaTable);
    convertToDFA();
    generateAcceptStates();
    generateDFA();
}
/*Method to convert vector of NFA transitions to map of map*/
void DFA::mappingTransitionTable(vector<transition> nfaTable)
{

    for(int i=0; i<nfaTable.size(); i++)
    {
        struct transition t = nfaTable[i];
        map<int, map<string, vector<int> > >::iterator it ;
        it = nfaTransitionsTable.find(t.fromState);
        if(it ==nfaTransitionsTable.end())
        {
            map<string, vector<int>> n;
            n[t.trans_symbol].push_back(t.toState);
            nfaTransitionsTable[t.fromState] = n;
        }
        else
        {
            nfaTransitionsTable[t.fromState][t.trans_symbol].push_back(t.toState);
        }

    }



}
/*Method to generate DFA transitions from NFA transitions*/
void DFA::convertToDFA()
{
    queue<vector<int>> state_queue;
    /*get epsilon closure of NFA start state and push it to the queue */
    state_queue.push( getEpsilonClosure(nfaStartState) );
    /*mark the epsilon closure of NFA start state that is checked*/
    checked_states[getEpsilonClosure(nfaStartState)] = true;
    /* loop on each DFA state after getting its epsilon closure and
    find its transition with each symbol,if DFA state goes to new state
    then get its epsilon closure, mark it and add it to the queue*/

    while( !state_queue.empty() )
    {
        vector<int> cur_state = state_queue.front();
        state_queue.pop();

        /*get the next states from the current state on each symbol*/
        for(int i = 0; i < symbols.size(); i++)
        {
            set<int> next;
            for(int j=0; j<cur_state.size(); j++)
            {
                vector<int> s = getNextState(cur_state[j],symbols[i]);
                copy( s.begin(), s.end(), std::inserter( next, next.end() ) );
            }

            vector<int> nextState;
            nextState.assign(next.begin(),next.end());

            nextState = getEpsilonClosure(nextState);

            if(checked_states.find(nextState) == checked_states.end())
            {
                state_queue.push(nextState);
                checked_states[nextState] = true;

            }
            /*add the new transition to the DFA transition table*/
            dfaTransitionTable[cur_state][symbols[i]] = nextState;
        }
    }


}
/*method to get the state that the given state goes to on the given symbol*/
vector<int> DFA::getNextState(int curState, string symbol)
{
    return nfaTransitionsTable[curState][symbol];
}
/*Method to get the epsilon closure of the state*/
vector<int> DFA::getEpsilonClosure(vector<int> state)
{
    map<int, bool> closure;
    for(int i = 0; i < state.size(); i++)
    {
        closure[state[i]] = true;
    }
    /*loop to get all states that the each NFA state go to on LAMDA symbol*/
    for(int i = 0; i < state.size(); i++)
    {
        if( isTransitionExist(state[i], "\\L") )
        {
            vector<int> new_state = nfaTransitionsTable[state[i]]["\\L"];
            for(int j=0; j<new_state.size(); j++)
            {
                if(closure.find(new_state[j]) == closure.end())
                {
                    state.push_back( new_state[j]);
                    closure[new_state[j]] = true;
                }
            }
        }
    }

    return state;
}

/*Method to check if there is transition from the given state on the given symbol*/
bool DFA::isTransitionExist(int subState, string ch)
{
    map<int, map<string, vector<int>>>::iterator it;
    it = nfaTransitionsTable.find(subState);
    if(it != nfaTransitionsTable.end())
    {
        map<string, vector<int>>m = it->second;

        if(m.find( ch) == m.end())
            return false;
        else
            return true;
    }

}
/*Method to generate Accepted states and add to it the token type that it accepts*/
void DFA::generateAcceptStates()
{
    for(map<vector<int>, bool>::iterator iter = checked_states.begin(); iter != checked_states.end(); iter++)
    {
        vector<int> v = iter->first;
        for(int j = 0; j < v.size(); ++j)
        {
            for(int k = 0; k < nfaAcceptedStates.size(); ++k)
            {

                if(v[j] == nfaAcceptedStates[k]->getStateNum())
                {
                    pair<vector<int>,string> s;
                    s.first = v;
                    s.second=nfaAcceptedStates[k]->getTokenType();

                    dfaAcceptedStates.push_back(s);

                }
            }
        }
    }
}
/*Method to create the DFA transition Table*/
void DFA::generateDFA()
{
    int state_cnt = 0;

    for(map<vector<int>, bool>::iterator iter = checked_states.begin(); iter != checked_states.end(); iter++)
    {
        statesToInt[iter->first] = state_cnt;
        dfaStates.push_back(state_cnt);
        state_cnt++;
    }

    dfaStartState = getEpsilonClosure(nfaStartState) ;
    std::map<std::vector<int>, int>::iterator it;
    it = statesToInt.find(dfaStartState);
    if(it != statesToInt.end())
    {
        startState = it ->second;
    }

    for(int i=0; i<dfaAcceptedStates.size(); i++)
    {
        std::map<std::vector<int>, int>::iterator it;
        it = statesToInt.find(dfaAcceptedStates[i].first);
        if(it != statesToInt.end())
        {
            bool found = false;
            for(int j=0; j<acceptedStates.size(); j++)
            {
                if(acceptedStates[j].getStateNum()==it ->second)
                {
                    string type = acceptedStates[j].getTokenType();
                    if((type=="id") && (type != dfaAcceptedStates[i].second))
                    {
                        acceptedStates[j].setTokenType(dfaAcceptedStates[i].second);
                    }

                    found = true;
                    break;
                }

            }
            if(!found)
            {
                AcceptedState s;
                s.setStateNum(it ->second);
                s.setTokenType(dfaAcceptedStates[i].second);
                acceptedStates.push_back(s);
            }


        }
    }

//    cout<<"Start state: "<<startState<<endl;
//    cout<<"Number of DFA states "<< dfaStates.size()<<endl;
//    cout<<"Accepted States: "<<endl;
//    for(int i=0; i<acceptedStates.size(); i++)
//    {
//        cout<<acceptedStates[i].getStateNum()<<" "<<acceptedStates[i].getTokenType()<<endl;
//    }

    for(map<vector<int>, bool>::iterator iter = checked_states.begin(); iter != checked_states.end(); iter++)
    {
        std::map<std::vector<int>, int>::iterator it;
        it = statesToInt.find(iter->first);
        int from ;
        if(it != statesToInt.end())
        {
            from = it ->second;
        }
        for(int j = 0; j < symbols.size(); ++j)
        {
            std::map<std::vector<int>, int>::iterator it1;
            it1 = statesToInt.find(dfaTransitionTable[iter->first][symbols[j]]);
            if(it1 != statesToInt.end())
            {
                dfaTable[from][symbols[j]] = it1->second;

            }
        }

    }
    ofstream myfile;
    myfile.open ("DFATable.txt");
    for(map<int,map<string,int>>::iterator it = dfaTable.begin();
            it != dfaTable.end(); ++it)
    {

        map<string,int> var = it-> second;

        for(map<string,int>::iterator it1 = var.begin();
                it1 != var.end(); ++it1)
        {
            //std::cout << it->first <<" "<<it1->first<< " "<<it1->second  << "\n";
            myfile<< it->first <<" "<<it1->first<< " "<<it1->second  << "\n";

        }
    }
    myfile.close();

}
map<int,map<string,int>> DFA::getDfaTable()
{
    return dfaTable;
}
int DFA::getStartState()
{
    return startState;
}
vector<AcceptedState> DFA::getAcceptedStates()
{

    return acceptedStates;
}
int DFA::getNumberOfDFAStates()
{

    return dfaStates.size();
}
set<int> DFA::getDFAStates()
{
    set<int> s;
    copy( dfaStates.begin(), dfaStates.end(), std::inserter( s, s.end() ) );

    return s;
}
