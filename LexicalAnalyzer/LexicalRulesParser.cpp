//
// Created by meritv on 3/20/19.
//

#include "LexicalRulesParser.h"
#include "global.h"

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
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}

void parseRulesFile(ifstream &rulesFile) {
    if (!rulesFile) {
        cerr << "Unable to open lexical rules file";
        exit(1);
    }
    string line;
    vector<vector<string>> regularDefinitions;

    /* TODO mention assumption in report */
    /* All regular definitions are written in the standard form of regex*/

    while (getline(rulesFile, line)) {
        if (regex_match(line, regex("[a-zA-Z](\\w)*(\\s)*=(\\s)*(\.)+"))) {  /* Regular Definition */
            line.erase(remove_if(line.begin(), line.end(),
                    [](unsigned char x){return std::isspace(x);}), line.end());
            string key = strtok((char *)line.c_str(), "=");
            string value = strtok(NULL, "=");
            for (int i = 0; i < regularDefinitions.size(); i++) {
                findAndReplaceAll(value, regularDefinitions[i][0], regularDefinitions[i][1]);
            }
            regularDefinitions.insert(regularDefinitions.begin(), {key, value});
        } else if (regex_match(line, regex("[a-zA-Z](\\w)*(\\s)*(:)(\\s)*(\.)+"))) { /* Regular definition */
            line.erase(remove_if(line.begin(), line.end(),
                                 [](unsigned char x){return std::isspace(x);}), line.end());
            string key = strtok((char *)line.c_str(), ":");
            string value = strtok(NULL, ":");
            for (int i = 0; i < regularDefinitions.size(); i++) {
                findAndReplaceAll(value, regularDefinitions[i][0], regularDefinitions[i][1]);
            }
            regularExpressions.insert(pair<string,string>(key, value));
        } else if(regex_match(line, regex("\\{(\\s)*(\.)+(\\s)*\\}"))) { /* keywords */
            char* kw = strtok((char *)line.c_str(), " {}");
            while (kw != NULL) {
                keywords.push_back(kw);
                kw = strtok(NULL, " {}");
            }
        } else if (regex_match(line, regex("\\[(\\s)*(\.)+(\\s)*\\]"))) { /* Punctuations */
            char *kw = strtok((char *)line.c_str(), " []");
            while (kw != NULL) {
                punctuations.push_back(kw);
                kw = strtok(NULL, " []");
            }
        } else {
            printf("ERROR \" %s \" unknown type of tokens!\n", line);
        }
    }
}