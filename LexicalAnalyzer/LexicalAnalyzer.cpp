//
// Created by meritv on 3/20/19.
//

#include <iostream>
#include <regex>
#include "LexicalAnalyzer.h"
#include "LexicalRulesParser.h"
#include "global.h"
#include "NFA.h"
#include "ReToNFA.h"
#include "UnionedNFA.h"

using namespace std;

map <string, string> regularExpressions;
vector<string> keywords;
vector<string> punctuations;

int main(int argc,char* argv[]) {
    if (argc < 2) {
        cout << "Wrong number of files passed\n";
    }
    ifstream file(argv[1]);
    parseRulesFile(file);
    UnionedNFA unionedNFA = UnionedNFA();
    NFA nfa = unionedNFA.getNFA();
    vector<int> acceptanceStates = unionedNFA.getAcceptanceStates();
    nfa.printTransitions();

}