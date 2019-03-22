//
// Created by meritv on 3/20/19.
//

#include "LexicalRulesParser.h"

#include <stdio.h>
#include <iostream>
#include <map>
#include <cstring>
#include <string.h>
#include <algorithm>
#include <regex>
void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
    // Get the first occurrence
    size_t pos = data.find(toSearch);

    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos =data.find(toSearch, pos + replaceStr.size());
    }
}

void parseRulesFile(ifstream &rulesFile) {
    if (!rulesFile) {
        cerr << "Unable to open lexical rules file";
        exit(1);
    }
    string line;
    vector<map<string, string>> tokens_types;
    map <string, string> regularDefinitions;
    map <string, string> regularExpressions;
    vector<string> keywords;
    vector<char *> punctuations;

    /* TODO mention assumption in report */
    /* All regular definitions are written in the standard form of regex*/
    char reserved_symbols[] = {'(', ')', '|', '*', '+', '-'};
    while (getline(rulesFile, line)) {
        if (regex_match(line, regex("[a-zA-Z](\\w)*(\\s)*=(\\s)*(\.)+"))) {  /* Regular Definition */
            line.erase(remove_if(line.begin(), line.end(),
                    [](unsigned char x){return std::isspace(x);}), line.end());
            string key = strtok((char *)line.c_str(), "=");
            string value = strtok(NULL, "=");
            for (auto const& x : regularDefinitions)
            {
                findAndReplaceAll(value, x.first, x.second);
            }
            regularDefinitions.insert(pair<string,string>(key, value));
        } else if (regex_match(line, regex("[a-zA-Z](\\w)*(\\s)*:(\\s)*(\.)+"))) { /* Regular definition */
            //line.erase(remove_if(line.begin(), line.end(),
              //                   [](unsigned char x){return std::isspace(x);}), line.end());
            string key = strtok((char *)line.c_str(), ":");
            string value = strtok(NULL, ":");
            for (auto const& x : regularDefinitions)
            {
                findAndReplaceAll(value, x.first, x.second);
            }
            regularExpressions.insert(pair<string,string>(key, value));
        } else if(regex_match(line, regex("\\{(\\s)*(\.)+(\\s)*\\}"))) { /* keywords */
            string kw = strtok((char *)line.c_str(), " {}");
            while (!kw.empty()) {
                keywords.push_back(kw);
                kw = strtok(NULL, " {}");
            }
        } else if (regex_match(line, regex("\\[(\\.)+\\]"))) { /* Punctuations */
            char *kw = strtok((char *)line.c_str(), " []\\");
            while (kw != NULL) {
                keywords.push_back(kw);
                kw = strtok(NULL, " []\\");
            }
        }
    }
    printf("Definitions\n");
    for (auto const& x : regularDefinitions)
    {
        printf("%s : %s\n", x.first.c_str(), x.second.c_str());
    }
    printf("Expressions\n");
    for (auto const& x : regularExpressions)
    {
        printf("%s : %s\n", x.first.c_str(), x.second.c_str());
    }
    printf("Keywords\n");
    for (int i = 0; i < keywords.size(); ++i) {
        printf("%s\n", keywords.at(i).c_str());
    }
    printf("Punctuations\n");
    for (int j = 0; j < punctuations.size(); ++j) {
        printf("%s\n", punctuations.at(j));
    }
}