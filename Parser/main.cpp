#include <iostream>
#include <string>
#include <vector>
#include "ParsingCFG.h"
#include "Production.h"
#include "PredictiveParser.h"
#include "ParserTable.h"
#include "LeftDerivation.h"
//int main()
//{
//    ParsingCFG p ;
//    vector<Production> productions;
//    set<string> b= p.terminals;
//    productions=p.getProductions("C:/Users/omid/Desktop/Parser/CFG.txt");
//    PredictiveParser predictiveParser =  PredictiveParser(productions, p.terminals,p.startNonTerminal);
//    ParserTable *table = predictiveParser.table;
//    table->printTable();
//    LeftDerivation l;
//    l.set_terminals(predictiveParser.get_terminals());
//    l.set_nonTerminals(predictiveParser.get_nonTerminals());
//    l.derive(table->getTable(),p.startNonTerminal);
//
//    return 0;
//}
