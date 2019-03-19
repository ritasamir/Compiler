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

}

//what i need
std::vector<string> nfaStates;
std::map<string, std::map<string, std::vector<string> > > nfaTransitionsTable;
string nfaStartState;
std::vector<string> nfaAcceptedStates;
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
 nfaStates.push_back("A");
    nfaStates.push_back("B");
    nfaStates.push_back("C");
    nfaStates.push_back("D");
   symbols.push_back("0");
    symbols.push_back("1");

    nfaStartState ="A";
    nfaAcceptedStates.push_back("B");
        nfaAcceptedStates.push_back("C");

    for(int i = 0; i < nfaStates.size(); ++i)
    {
        nfaTransitionsTable[nfaStates[i]]["L"] = std::vector<string>();
        for(int j = 0; j < symbols.size(); ++j)
        {
            nfaTransitionsTable[nfaStates[i]][symbols[j]] = std::vector<string>();
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
    nfaTransitionsTable["A"]["0"].push_back("B");
    nfaTransitionsTable["A"]["0"].push_back("C");
    nfaTransitionsTable["B"]["1"].push_back("A");
    nfaTransitionsTable["B"]["1"].push_back("C");
    nfaTransitionsTable["C"]["0"].push_back("D");
    nfaTransitionsTable["C"]["1"].push_back("D");
    nfaTransitionsTable["D"]["0"].push_back("B");
    nfaTransitionsTable["D"]["1"].push_back("D");

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
    std::vector<string> container= std::vector<string>();
    	container.push_back("deadState");

	dfaStates.push_back(container);

    for(int i=0; i<nfaStates.size(); ++i)
    {
        		container.clear();
        getEpsilonClosure(container, nfaStates[i]);

        if(!container.empty())
        {    int noTransitions=0;
        for(int j = 0; j < symbols.size(); ++j)
        {
            if(getNextState(nfaStates[i], symbols[j]).empty()){
                noTransitions++;
            }
        }
        if(!(noTransitions==symbols.size() && getNextState(nfaStates[i], "L").empty())){

            std::set<string> s( container.begin(), container.end() );
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

                std::set<string> s( container.begin(), container.end() );
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
                std::set<string> s( container.begin(), container.end() );
                container.assign( s.begin(), s.end() );
                addToStates(container);
            }


        }

    }
}


void DFA::getEpsilonClosure(std::vector<string> &container, string state)
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
    std::vector<string> container;
    for(int i = 0; i < dfaStates.size(); ++i)
    {
        for(int j = 0; j < symbols.size(); ++j)
        {
            container = std::vector<string>();

            for(int k = 0; k < dfaStates[i].size(); ++k)
            {

                for(int h = 0; h < getNextState(dfaStates[i][k], symbols[j]).size(); ++h)
                {
                    getEpsilonClosure(container, getNextState(dfaStates[i][k], symbols[j])[h]);
                }
            }
           if(container.empty())
            {
                container.push_back("deadState");
            }

            std::set<string> s( container.begin(), container.end() );
            container.assign( s.begin(), s.end() );
            addToStates(container);
            dfaTransitionTable[dfaStates[i]][symbols[j]] = container;
        }
    }
}



std::vector<string> DFA::getNextState(string curState, string symbol)
{
    return nfaTransitionsTable[curState][symbol];
}

void DFA::addToStates(std::vector<string> &container)
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
