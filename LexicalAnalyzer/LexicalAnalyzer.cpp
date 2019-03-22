//
// Created by meritv on 3/20/19.
//

#include <iostream>
#include <regex>
#include "LexicalAnalyzer.h"
#include "LexicalRulesParser.h"

using namespace std;
int main(int argc,char* argv[]) {
    if (argc < 2) {
        cout << "Wrong number of files passed\n";
    }
    ifstream file(argv[1]);
    parseRulesFile(file);
}