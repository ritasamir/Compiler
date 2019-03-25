#include "DFA.h"
#include <queue>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
using std::string;
using namespace std;

DFA::DFA()
{
    test();
    generateDFAStates();
    generateDFATransitionTable();
    generateStartState();
    generateAcceptStates();
    mapping();

}

//what i need
vector<int> nfaStates;
std::map<int, std::map<string, std::vector<int> > > nfaTransitionsTable;
int nfaStartState;
std::vector<int> nfaAcceptedStates;
void DFA::test()
{
    /*
    nfaStates.push_back("A");
    nfaStates.push_back("B");
    nfaStates.push_back("C");
    nfaStates.push_back("D");
    nfaStates.push_back("E");
    nfaStates.push_back("F");
    nfaStates.push_back("G");
    */
    nfaStates.push_back(1);
    nfaStates.push_back(2);
    nfaStates.push_back(3);
    nfaStates.push_back(4);
    symbols.push_back("0");
    symbols.push_back("1");

    nfaStartState =1;
    nfaAcceptedStates.push_back(2);
    nfaAcceptedStates.push_back(3);

    for(int i = 0; i < nfaStates.size(); ++i)
    {
        nfaTransitionsTable[nfaStates[i]]["L"] = std::vector<int>();
        for(int j = 0; j < symbols.size(); ++j)
        {
            nfaTransitionsTable[nfaStates[i]][symbols[j]] = std::vector<int>();
        }
    }
    /*
        nfaTransitionsTable["A"]["L"].push_back("B");
        nfaTransitionsTable["A"]["L"].push_back("C");
        nfaTransitionsTable["A"]["L"].push_back("E");
        nfaTransitionsTable["B"]["0"].push_back("D");
        nfaTransitionsTable["B"]["1"].push_back("D");
        nfaTransitionsTable["B"]["L"].push_back("C");
        nfaTransitionsTable["C"]["0"].push_back("D");
        nfaTransitionsTable["C"]["L"].push_back("E");
        nfaTransitionsTable["D"]["0"].push_back("E");
        nfaTransitionsTable["D"]["1"].push_back("F");
        nfaTransitionsTable["D"]["L"].push_back("G");
        nfaTransitionsTable["E"]["0"].push_back("E");
        nfaTransitionsTable["E"]["L"].push_back("B");
        nfaTransitionsTable["F"]["1"].push_back("E");
        nfaTransitionsTable["F"]["L"].push_back("G");
    */
    nfaTransitionsTable[1]["0"].push_back(2);
    nfaTransitionsTable[1]["0"].push_back(3);
    nfaTransitionsTable[2]["1"].push_back(1);
    nfaTransitionsTable[2]["1"].push_back(3);
    nfaTransitionsTable[3]["0"].push_back(4);
    nfaTransitionsTable[3]["1"].push_back(4);
    nfaTransitionsTable[4]["0"].push_back(2);
    nfaTransitionsTable[4]["1"].push_back(4);

}
void DFA::printDFA()
{
    std::ofstream writer;

    writer.open("output.DFA");

    //output the states to file
    int i, j;
    for(i = 0; i < (int)dfaStates.size(); ++i)
    {
        writer << "{";
        for(j = 0; j < (int)dfaStates[i].size() - 1; ++j)
        {
            writer << dfaStates[i][j];
            writer << ",";
        }
        writer << dfaStates[i][j];
        writer << "}\t";
    }
    writer << "\n";

    //output the symbols to file
    for(i = 0; i < (int)symbols.size(); ++i)
    {
        writer << symbols[i] << "\t";
    }
    writer << "\n";

    //output start state
    writer << "{";
    for(i = 0; i < (int)dfaStartState.size(); ++i)
    {
        writer << dfaStartState[i];
        if(i != (int)(dfaStartState.size() - 1))
        {
            writer << ",";
        }
    }
    writer << "}\n";

    //outputs the accept states to file
    for(i = 0; i < (int)dfaAcceptedStates.size(); ++i)
    {
        writer << "{";
        for(j = 0; j < (int)dfaAcceptedStates[i].size() - 1; ++j)
        {
            writer << dfaAcceptedStates[i][j];
            writer << ",";
        }
        writer << dfaAcceptedStates[i][j];
        writer << "}\t";
    }
    writer << "\n";

    //outputs the transition function to file
    int k;
    for(i = 0; i < (int)dfaStates.size(); ++i)
    {
        for(j = 0; j < (int)symbols.size(); ++j)
        {
            writer << "{";
            for(k = 0; k < (int)dfaStates[i].size() - 1; ++k)
            {
                writer << dfaStates[i][k];
                writer << ",";
            }
            writer << dfaStates[i][k];
            writer << "}, ";
            writer << symbols[j] << " = {";

            for(k = 0; k < (int)dfaTransitionTable[dfaStates[i]][symbols[j]].size() - 1; ++k)
            {
                writer << dfaTransitionTable[dfaStates[i]][symbols[j]][k];
                writer << ",";
            }
            writer << dfaTransitionTable[dfaStates[i]][symbols[j]][k];
            writer << "}";
            writer << "\n";
        }

    }
    writer.close();
}

void DFA::generateDFAStates()
{
    std::vector<int> container= std::vector<int>();
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
            if(!(noTransitions==symbols.size() && getNextState(nfaStates[i], "L").empty()))
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
                std::set<int> s( container.begin(), container.end() );
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
    for(int i = 0; i < getNextState(state, "L").size(); ++i)
    {
        if(state != getNextState(state, "L")[i])
        {
            for(int j = 0; j < container.size(); ++j)
            {
                if(container[j] == getNextState(state, "L")[i])
                {
                    isDuplicate = true;
                }
            }
            if(!isDuplicate)
            {
                getEpsilonClosure(container, getNextState(state, "L")[i]);
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
    //map each vector of states to one state
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

    cout<<startState<<endl;
    for(int x:acceptedStates)
    {
        cout<<x<<" ";
    }
    cout<<endl;

//TODO resolve it
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

for(std::map<int,std::map<std::string,int>>::iterator it = dfaTable.begin();
    it != dfaTable.end(); ++it)
{
    std::cout << "start ";
    std::map<std::string,int> var = it-> second;
    for(std::map<std::string,int>::iterator it1 = var.begin();
    it1 != var.end(); ++it1){
        std::cout << it->first <<" "<<it1->first<< " "<<it1->second  << "\n";
    }
}


}

