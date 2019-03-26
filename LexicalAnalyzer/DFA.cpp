#include "DFA.h"
#include <queue>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "AcceptedState.h"
using std::string;
using namespace std;

DFA::DFA(vector<AcceptedState*> finalStates, vector<string> inputs,vector<transition> nfaTable)
{
    for(int i=0; i<412; i++)
    {
        nfaStates.push_back(i);
    }
    for(int i=0; i<finalStates.size(); i++)
    {
        nfaAcceptedStates.push_back(finalStates[i]->getStateNum());
    }
    symbols = inputs;
    symbols.erase(std::remove(symbols.begin(), symbols.end(), "\\L"), symbols.end());

    nfaStartState = 0;
    mappingTransitionTable(nfaTable);
    generateDFAStates();
    generateDFATransitionTable();
    generateStartState();
    generateAcceptStates();
    mapping();
    removeUnreachableStates();

}


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
void DFA::generateDFAStates()
{
    vector<int> container= vector<int>();
    container.push_back(0);

    dfaStates.push_back(container);

    for(int i=0; i<nfaStates.size(); ++i)
    {
        container.clear();
        getEpsilonClosure(container, nfaStates[i]);
        if(!container.empty())
        {

            int noTransitions=0;
            for(int j = 0; j < symbols.size(); ++j)
            {
                if(getNextState(nfaStates[i], symbols[j]).empty())
                {
                    noTransitions++;
                }
            }
            if(!(noTransitions==symbols.size() && getNextState(nfaStates[i], "\\L").empty()))
            {

                std::set<int> s( container.begin(), container.end() );
                container.assign( s.begin(), s.end() );
                addToStates(container);
            }
        }

        container.clear();
        for(int j = 0; j < symbols.size(); ++j)
        {

            for(int k = 0; k < getNextState(nfaStates[i], symbols[j]).size(); ++k)
            {
                getEpsilonClosure(container, getNextState(nfaStates[i], symbols[j])[k]);
            }
            if(!container.empty())
            {

                std::set<int> s( container.begin(), container.end() );
                container.assign( s.begin(), s.end() );
                addToStates(container);
            }

        }
    }

    int numOfStates = dfaStates.size();
    for(int i = 0; i < numOfStates; ++i)
    {
        for(int j = 0; j < symbols.size(); ++j)
        {
            container.clear();
            for(int k = 0; k < dfaStates[i].size(); ++k)
            {
                for(int h = 0; h < getNextState(dfaStates[i][k], symbols[j]).size(); ++h)
                {
                    getEpsilonClosure(container, getNextState(dfaStates[i][k], symbols[j])[h]);
                }
            }
            if(!container.empty())
            {
                set<int> s( container.begin(), container.end() );
                container.assign( s.begin(), s.end() );
                addToStates(container);
            }



        }

    }

}


void DFA::getEpsilonClosure(std::vector<int> &container, int state)
{
    bool isDuplicate = false;
    container.push_back(state);
    for(int i = 0; i < getNextState(state, "\\L").size(); ++i)
    {
        if(state != getNextState(state, "\\L")[i])
        {
            for(int j = 0; j < container.size(); ++j)
            {
                if(container[j] == getNextState(state, "\\L")[i])
                {
                    isDuplicate = true;
                }
            }
            if(!isDuplicate)
            {
                getEpsilonClosure(container, getNextState(state, "\\L")[i]);
            }
            else
            {
                isDuplicate = false;
            }
        }
    }

}
void DFA::generateDFATransitionTable()
{
    std::vector<int> container;
    for(int i = 0; i < dfaStates.size(); ++i)
    {
        for(int j = 0; j < symbols.size(); ++j)
        {
            container = std::vector<int>();

            for(int k = 0; k < dfaStates[i].size(); ++k)
            {

                for(int h = 0; h < getNextState(dfaStates[i][k], symbols[j]).size(); ++h)
                {
                    getEpsilonClosure(container, getNextState(dfaStates[i][k], symbols[j])[h]);
                }
            }
            if(container.empty())
            {
                container.push_back(0);
            }

            std::set<int> s( container.begin(), container.end() );
            container.assign( s.begin(), s.end() );
            addToStates(container);
            dfaTransitionTable[dfaStates[i]][symbols[j]] = container;
        }
    }
}



std::vector<int> DFA::getNextState(int curState, string symbol)
{
    return nfaTransitionsTable[curState][symbol];
}

void DFA::addToStates(std::vector<int> &container)
{

    for(int i = 0; i < (int)dfaStates.size(); ++i)
    {
        if(dfaStates[i] == container)
        {
            return;
        }
    }
    dfaStates.push_back(container);
}

void DFA::generateStartState()
{
    getEpsilonClosure(dfaStartState, nfaStartState);
}
void DFA::generateAcceptStates()
{
    bool acceptedState = false;
    for(int i = 0; i < dfaStates.size(); ++i)
    {
        for(int j = 0; j < dfaStates[i].size(); ++j)
        {
            for(int k = 0; k < nfaAcceptedStates.size(); ++k)
            {
                if(acceptedState)
                {
                    acceptedState = false;
                    break;
                }
                if(dfaStates[i][j] == nfaAcceptedStates[k])
                {
                    dfaAcceptedStates.push_back(dfaStates[i]);
                    acceptedState = true;
                    break;
                }
            }
        }
    }
}
void DFA::mapping()
{
    int i;
    for(i =0; i<dfaStates.size(); i++)
    {
        statesToInt[dfaStates[i]] = i;
    }
    std::map<std::vector<int>, int>::iterator it;
    it = statesToInt.find(dfaStartState);
    if(it != statesToInt.end())
    {
        startState = it ->second;
    }
    for(i=0; i<dfaAcceptedStates.size(); i++)
    {
        std::map<std::vector<int>, int>::iterator it;
        it = statesToInt.find(dfaAcceptedStates[i]);
        if(it != statesToInt.end())
        {
            acceptedStates .push_back(it ->second);
        }
    }

    cout<<"start state "<<startState<<endl;
    cout<<"accepted "<<endl;
    for(int x:acceptedStates)
    {
        cout<<x<<" ";
    }
    cout<<endl;

    for(int i = 0; i < dfaStates.size(); ++i)
    {
        std::map<std::vector<int>, int>::iterator it;
        it = statesToInt.find(dfaStates[i]);
        int from ;
        if(it != statesToInt.end())
        {
            from = it ->second;
        }
        for(int j = 0; j < symbols.size(); ++j)
        {
            std::map<std::vector<int>, int>::iterator it1;
            it1 = statesToInt.find(dfaTransitionTable[dfaStates[i]][symbols[j]]);
            if(it1 != statesToInt.end())
            {
                dfaTable[from][symbols[j]] = it1->second;

            }
        }

    }

}
void DFA::removeUnreachableStates()
{
    set<int> from;
    set<int> to;
    to.insert(startState);
    for(map<int,map<string,int>>::iterator it = dfaTable.begin();
            it != dfaTable.end(); ++it)
    {
        map<string,int> var = it-> second;
        from.insert(it->first);

        for(map<string,int>::iterator it1 = var.begin();
                it1 != var.end(); ++it1)
        {
            to.insert(it1->second);

        }
    }

    set<int> result;
    set_difference(from.begin(), from.end(), to.begin(), to.end(),
                   std::inserter(result, result.end()));
    numOfDFAStates = dfaStates.size()-result.size();
    DFAStates = to;

    cout<<"number of dfa states " <<numOfDFAStates<<endl;
    for(int x:DFAStates){
        cout<<x<<endl;
    }
    for(int x: result)
    {
        dfaTable.erase(x);

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
            // std::cout << it->first <<" "<<it1->first<< " "<<it1->second  << "\n";
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
vector<int> DFA::getAcceptedStates()
{

    return acceptedStates;
}
int DFA::getNumberOfDFAStates()
{

    return numOfDFAStates;
}
set<int> DFA::getDFAStates(){
return DFAStates;
}

