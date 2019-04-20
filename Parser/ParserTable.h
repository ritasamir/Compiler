#ifndef PARSERTABLE_H
#define PARSERTABLE_H


#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class ParserTable
{
public:
    ParserTable();
    void fillTable(string nonTerminal, string terminal, vector<string> production);
    vector<string> getProduction(string nonTerminal, string terminal);
    bool isEmpty(string nonTerminal, string terminal);
    void printTable();
    map<string, map<string,vector<string>>> getTable();

private:
    map<string, map<string,vector<string>>> parserTable;
};

#endif // PARSERTABLE_H
