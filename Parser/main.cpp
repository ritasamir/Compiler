#include <iostream>
#include <string>
#include <vector>
#include "ParsingCFG.h"
#include "Production.h"
#include "PredictiveParser.h"
#include "ParserTable.h"
int main() {
    ParsingCFG p ;
    vector<Production> productions;
    productions=p.getProductions("E:/study/Parser_Generator/CFG.txt");
    PredictiveParser predictiveParser =  PredictiveParser(productions, p.terminals);
    ParserTable *table = predictiveParser.table;
    table->printTable();
    return 0;
}
