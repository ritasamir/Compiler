#ifndef LEFTDERIVATION_H
#define LEFTDERIVATION_H
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <fstream>
using namespace std;
class LeftDerivation
{
public:
    void derive(map<string, map<string,vector<string>>> table, string start);
    void set_terminals(set<string> input);
    void set_nonTerminals(set<string> input);
    bool is_terminal(string token);
    bool is_nonTerminal(string token);

private:
    set<string> terminals;
    set<string> nonTerminals;
};

#endif // LEFTDERIVATION_H
