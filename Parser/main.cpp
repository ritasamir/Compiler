#include <iostream>
#include <string>
#include <vector>
#include "ParsingCFG.h"
#include "Production.h"

int main() {
    ParsingCFG p ;
    vector<Production> productions;
    productions=p.getProductions("/home/rita/CLionProjects/parser/CFG.txt");
    return 0;
}