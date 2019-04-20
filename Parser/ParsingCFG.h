//
// Created by rita on 12/04/19.
//

#ifndef PARSER_PARSINGFILE_H
#define PARSER_PARSINGFILE_H
#include <vector>
#include <string>
#include <set>
#include <stack>
#include "Production.h"

using namespace std;


class ParsingCFG
{

public:
    vector<Production> getProductions(string fileName);

    string startNonTerminal;

    set<string> terminals;

    void printProductions(vector<Production> productions);

private:

    stack <string> parseFile(string file);

    vector<Production> addProduction(vector<Production> productions, string lhs, string rhs);

    vector<Production> eliminateLeftReccursion(vector<Production> productions);

    void eliminateLeftReccursionHelper(vector<Production> *productions, vector<Production>::iterator iterator, string basicString,
                                       string string1);

    vector<Production> eliminateLeftFactoring(vector<Production> productions);
};


#endif //PARSER_PARSINGFILE_H
