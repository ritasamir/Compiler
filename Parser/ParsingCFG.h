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


class ParsingCFG {

public:
    vector<Production> getProductions(string fileName);
    set<string> terminals;

private:

    stack <string> parseFile(string file);

    vector<Production> addProduction(vector<Production> productions, string lhs, string rhs);
};


#endif //PARSER_PARSINGFILE_H
