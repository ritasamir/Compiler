//
// Created by meritv on 3/22/19.
//

#ifndef COMPILER_GLOBAL_H
#define COMPILER_GLOBAL_H

#include <map>
#include <vector>
#include "string"

using namespace std;
extern map <string, string> regularExpressions;
extern vector<string> keywords;
extern vector<string> punctuations;
struct transition {
    int fromState;
    int toState;
    string trans_symbol;
};

#endif //COMPILER_GLOBAL_H
