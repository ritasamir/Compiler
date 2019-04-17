#ifndef PREDICTIVEPARSER_H
#define PREDICTIVEPARSER_H
#include "Production.h"
#include <vector>
#include "ParserTable.h"
#include <set>
class PredictiveParser
{
    public:
        PredictiveParser(vector<Production> productions, set<string> terminals);
        map<string, set<string>> getFirstSet();
        map<string, set<string>> getFollowSet();
        ParserTable *table;

    private:

        bool isContainEpsilon(string token);
        void generateFirst();
        void createFirstSet(string nonTerminal);
        bool isNonTerminal(string p);
        void generateFollow();
        void createFollowSet(string nonTerminal);
        void addDependencies();
        bool isHasEpsilon(string nonTerminal);
        void constructParserTable();

        vector<string> hasEpsilon;
        vector<Production> productions;
        map<string, vector<vector<string>>> mergedProductions;
        set<string> nonTerminals;
        set<string> inputs;
        string startNonTerminal;
        map<string, set<string>> firstMap;
        map<string, set<string>> followMap;
        map<string,vector<string>> dependentOn;
        map<string, bool> followSolved;
};

#endif // PREDICTIVEPARSER_H
