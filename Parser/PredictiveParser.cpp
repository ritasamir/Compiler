#include "PredictiveParser.h"
#include <iostream>
#include <algorithm>
#include "ParserTable.h"
using namespace std;

PredictiveParser::PredictiveParser(vector<Production> rules, set<string> terminals, string start)
{
    table = new ParserTable();
    productions = rules;
    startNonTerminal = start;
    inputs = terminals;
    inputs.insert("$");
    for(int i=0; i<productions.size(); i++)
    {
        nonTerminals.insert(productions[i].LHS);
        mergedProductions[productions[i].LHS].push_back(productions[i].RHS);

    }
    /*
    for(map<string,vector<vector<string>>>::iterator it=mergedProductions.begin(); it != mergedProductions.end();it++){
        vector<vector<string>> s = it->second;
        cout<<"Non terminal  "<<it->first<<endl;
        for(vector<string> t: s){
            for(string z: t){
                cout<<z<<"  ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
    */

    generateFirst();
    cout<<endl;

    cout<<"-----------------------FIRST-----------------------"<<endl;
    for (map<string, set<string>>::iterator it = firstMap.begin(); it != firstMap.end(); ++it)
    {
        set<string> set = it->second;

        cout << "non terminal: " << it->first <<"  "<<set.size()<< endl;
        std::set<string>::iterator iter;
        for (iter = set.begin(); iter != set.end(); ++iter)
        {
            cout << *iter << '\t';
        }
        cout << endl;
    }
    cout<<endl;

    generateFollow();
    addDependencies();
    cout << "-----------------------FOLLOW--------------------" << endl;
    for (map<string, set<string>>::iterator it = followMap.begin(); it != followMap.end(); ++it)
    {
        set<string> set = it->second;

        cout << "non terminal: " << it->first << endl;
        std::set<string>::iterator iter;
        for (iter = set.begin(); iter != set.end(); ++iter)
        {
            cout << *iter << '\t';
        }
        cout << endl;
    }
    cout<<"--------------------------------------------------"<<endl;

    constructParserTable();
}
void PredictiveParser::generateFirst()
{
    for(set<string>::iterator it = nonTerminals.begin(); it != nonTerminals.end(); it++)
    {

        createFirstSet(*it);

    }

}
void PredictiveParser::createFirstSet(string nonTerminal)
{

    if(!firstMap[nonTerminal].empty()) return;
    vector<vector<string>> nonTerminalProductions = mergedProductions[nonTerminal];
    int counter = 0;
    for(vector<string> production: nonTerminalProductions)
    {
        string first = production[0];
        if(first == "\\L" || !isNonTerminal(first))
        {
            firstMap[nonTerminal].insert(first);
            if(first == "\\L")
            {
                hasEpsilon.push_back(nonTerminal);
            }
            else
            {
                if(table->isEmpty(nonTerminal,first))
                {
                    table->fillTable(nonTerminal, first, production);

                }
                else if(table->getProduction(nonTerminal,first) != production)
                {

                    cout << "There is ambiguity at the production of ";
                    cout << "NonTerminal: " << nonTerminal << endl;
                    cout << " At Terminal: " << first << endl;
                }

            }

        }
        else
        {

            createFirstSet(first);
            set<string> s = firstMap[first];
            for(set<string>::iterator it = s.begin(); it != s.end(); it++)
            {
                firstMap[nonTerminal].insert(*it);
                if(*it == "\\L")
                {
                    hasEpsilon.push_back(nonTerminal);
                }
                else
                {
                    if(table->isEmpty(nonTerminal,*it))
                    {
                        table->fillTable(nonTerminal, *it, production);

                    }
                    else if(table->getProduction(nonTerminal,*it) != production)
                    {
                        cout << "There is ambiguity at the production of ";
                        cout << "NonTerminal: " << nonTerminal << endl;
                        cout << " At Terminal: " << *it << endl;
                    }

                }

            }

            while( counter < production.size() && isContainEpsilon(first))
            {
                first = production[++counter];
                if(first == "\\L" || !isNonTerminal(first))
                {
                    firstMap[nonTerminal].insert(first);
                    if(first == "\\L")
                    {
                        hasEpsilon.push_back(nonTerminal);
                    }
                    else
                    {
                        if(table->isEmpty(nonTerminal,first))
                        {
                            table->fillTable(nonTerminal, first, production);

                        }
                        else if(table->getProduction(nonTerminal,first) != production)
                        {
                            cout << "There is ambiguity at the production of ";
                            cout << "NonTerminal: " << nonTerminal << endl;
                            cout << " At Terminal: " << first << endl;
                        }

                    }
                    break;
                }
                createFirstSet(first);
                set<string> s1 = firstMap[first];
                for(set<string>::iterator it = s1.begin(); it != s1.end(); it++)
                {
                    firstMap[nonTerminal].insert(*it);
                    if(*it == "\\L")
                    {
                        hasEpsilon.push_back(nonTerminal);
                    }
                    else
                    {
                        if(table->isEmpty(nonTerminal,*it))
                        {
                            table->fillTable(nonTerminal, *it, production);

                        }
                        else if(table->getProduction(nonTerminal,*it) != production)
                        {
                            cout << "There is ambiguity at the production of ";
                            cout << "NonTerminal: " << nonTerminal << endl;
                            cout << " At Terminal: " << *it << endl;
                        }

                    }
                }
            }
        }
    }



}


bool PredictiveParser::isContainEpsilon(string token)
{
    set<string> s = firstMap[token];
    for(set<string>::iterator iter = s.begin(); iter != s.end(); iter++)
    {
        if(*iter == "\\L")
            return true;
    }
    return false;
}
void PredictiveParser::generateFollow()
{

    //startNonTerminal= productions[productions.size()-1].LHS;
    followMap[startNonTerminal].insert("$");

    for(set<string>::iterator it = nonTerminals.begin(); it != nonTerminals.end(); it++)
    {
        createFollowSet(*it);
    }
}
void PredictiveParser::createFollowSet(string nonTerminal)
{
    for(map<string,vector<vector<string>>>::iterator it=mergedProductions.begin(); it != mergedProductions.end(); ++it)
    {
        vector<vector<string>> p = it->second;
        for(int i=0; i<p.size(); i++)
        {
            vector<string> production = p[i];
            for(int j=0; j<production.size(); j++)
            {
                if(production[j] == nonTerminal && (j== production.size()-1) && followSolved[it->first] && nonTerminal != it->first)
                {
                    dependentOn[nonTerminal].push_back(it->first);
                    dependentOn[it->first].push_back(nonTerminal);
                    for(int k=0; k<dependentOn[nonTerminal].size(); k++)
                    {
                        dependentOn[it->first].push_back(dependentOn[nonTerminal][k]);
                    }
                    for(int k=0; k<dependentOn[it->first].size(); k++)
                    {
                        dependentOn[nonTerminal].push_back(dependentOn[it->first][k]);
                    }
                }
                if(production[j] == nonTerminal && (j== production.size()-1) && !followSolved[it->first] && nonTerminal != it->first)
                {

                    followSolved[nonTerminal] = true;
                    if(followMap.find(it->first) == followMap.end())  createFollowSet(it->first);
                    followSolved[nonTerminal] = false;

                    set<string> followSet= followMap[it->first];
                    for(set<string>::iterator iter = followSet.begin(); iter != followSet.end(); iter ++)
                    {
                        followMap[nonTerminal].insert(*iter);
                    }
                }
                else if(production[j] == nonTerminal && (j+1) < production.size() && isNonTerminal(production[j+1]))
                {
                    set<string> firstOfNextToken = firstMap[production[j+1]];
                    for(set<string>::iterator itr = firstOfNextToken.begin(); itr != firstOfNextToken.end(); itr++)
                    {
                        if(*itr == "\\L")
                        {
                            followSolved[nonTerminal] = true;
                            if(followMap.find(production[j+1]) == followMap.end())
                            {
                                createFollowSet(production[j+1]);
                            }
                            followSolved[nonTerminal] = false;

                            if(followMap[production[j+1]].empty() || dependentOn.find(production[j+1]) != dependentOn.end())
                            {
                                dependentOn[nonTerminal].push_back(production[j+1]);
                                for(int g=0; g<dependentOn[production[j+1]].size(); g++)
                                {
                                    dependentOn[nonTerminal].push_back(dependentOn[production[j+1]][g]);
                                }
                            }
                            set<string> followS = followMap[production[j+1]];
                            for(set<string>::iterator it1 = followS.begin(); it1 != followS.end(); it1++)
                            {
                                followMap[nonTerminal].insert(*it1);
                            }

                        }
                        else
                        {
                            followMap[nonTerminal].insert(*itr);
                        }
                    }

                }
                else if(production[j] == nonTerminal && (j+1) < production.size() && !isNonTerminal(production[j+1]))
                {

                    followMap[nonTerminal].insert(production[j+1]);

                }


            }
        }


    }
}

void PredictiveParser::addDependencies()
{

    for(map<string,vector<string>>:: iterator it = dependentOn.begin(); it != dependentOn.end(); ++it)
    {
        vector<string> v = it->second;
        for(int i=0; i<v.size(); i++)
        {
            set<string> s = followMap[v[i]];
            for(set<string>::iterator iter = s.begin(); iter != s.end(); iter++)
            {
                followMap[it->first].insert(*iter);

            }
        }
    }

}

bool PredictiveParser::isNonTerminal(string p)
{
    set<string>::iterator iter;
    for (iter = nonTerminals.begin(); iter != nonTerminals.end(); ++iter)
    {

        if(*iter == p)
        {
            return true;
        }
    }
    return false;
}
void PredictiveParser::constructParserTable()
{
    vector<string> epsilonProduction;
    epsilonProduction.push_back("\\L");
    vector<string> synch;
    synch.push_back("synch");
    vector<string> error;
    error.push_back("error");
    for(int i = 0; i<hasEpsilon.size(); i++)
    {
        set<string> s = followMap[hasEpsilon[i]];
        for(set<string>::iterator it = s.begin(); it != s.end(); it++)
        {
            if(table->isEmpty(hasEpsilon[i], *it))
            {
                table->fillTable(hasEpsilon[i], *it, epsilonProduction);
            }
            else
            {
                cout << "epsilon  There is ambiguity at the production of ";
                cout << "NonTerminal: " << hasEpsilon[i] << endl;
                cout << " At Terminal: " << *it << endl;
            }
        }
    }
    for(set<string>::iterator it = nonTerminals.begin(); it != nonTerminals.end(); it++)
    {
        if(!isHasEpsilon(*it))
        {
            set<string> s = followMap[*it];
            for(set<string>::iterator iter = s.begin(); iter != s.end(); iter++)
            {
                if(table->isEmpty(*it, *iter))
                {
                    table->fillTable(*it, *iter, synch);
                }
            }
        }

    }

    for(set<string>::iterator it = nonTerminals.begin(); it != nonTerminals.end(); it++)
    {
        for(set<string>::iterator it1 = inputs.begin(); it1!= inputs.end(); it1++)
        {
            if(table->isEmpty(*it, *it1))
            {
                table->fillTable(*it, *it1, error);
            }
        }
    }


}
bool PredictiveParser::isHasEpsilon(string nonTerminal)
{
    for(int i = 0; i<hasEpsilon.size(); i++)
    {
        if(hasEpsilon[i]== nonTerminal) return true;
    }
    return false;
}


map<string, set<string>> PredictiveParser::getFirstSet()
{
    return firstMap;
}

map<string, set<string>> PredictiveParser::getFollowSet()
{
    return followMap;
}
set<string> PredictiveParser::get_nonTerminals(){
    return nonTerminals;
}
set<string> PredictiveParser::get_terminals(){
    return inputs;
}
