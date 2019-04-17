#include "ParsingCFG.h"
#include <regex>
#include <iostream>
#include <fstream>

using namespace std;

vector<Production> ParsingCFG::getProductions(string fileName){

    stack<string> lines = parseFile(fileName);
    vector<Production> productions;
    stack<string> s=lines;

    while (!s.empty())
    {
        string line = s.top();
        s.pop();
        string rhs="";
        string lhs="";
        int i=1;
        while(line.at(i)!='='){
            if(line.at(i)!=' ')
                lhs+=line.at(i);
            i++;
        }

        for(int j=i+1;j<line.size();j++) { //assuming no space bteween one production
            if(line.at(j)=='|') {
                productions= addProduction(productions,lhs,rhs);
                rhs="";
            }else {
                rhs+=string(1, line.at(j));
            }
        }
        if(rhs!="") {
            productions = addProduction(productions,lhs,rhs);
        }
    }

    cout<<"Production : "<<endl;
    for(Production l :  productions){
        cout<<l.LHS<<" ---> ";
        int i=1;
        for(string s:l.RHS){
            cout<<i<<"-"<<s<<" ";
            i++;
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<"Terminals : "<<endl;
    for(string s :terminals){
        cout<<s<<"  ";
    }
    cout<<endl;
    cout<<endl;
    cout<<"--------------------------------------------------"<<endl;

    //check mn size terminals m3 el ba2yen bs hya sa7  y3ni
    return productions;
}

stack<string> ParsingCFG::parseFile(string fileName) {
    ifstream file(fileName);
    string line;
    stack<string> lines;
    while (getline(file, line)) {
        if ( regex_match(line, regex("^#(\\s)*([a-zA-Z0-9_]+)(\\s)*=(\\s)*[\\x00-\\x7B}~]+")) ) {
            //  cout << line<<"  wa7da = wa7da"<<endl;
            lines.push(line);

        }else if(regex_match(line, regex("^#(\\s)*([a-zA-Z0-9_]+)(\\s)*=(\\s)*[\\x00-\\x7F]+"))){
            lines.push(line);
        }else if(regex_match(line, regex("^[\\x00-\\x7F]+"))) {
            //    cout << line << "  ba2et satr" << endl;
            string temp = lines.top();
            lines.pop();
            temp+=line;
            lines.push(temp);

        }else{
            cout<<line<<" o match"<<endl;
        }

    }
    return lines;
}

vector<Production> ParsingCFG::addProduction(vector<Production> productions, string lhs, string rhs) {
    Production p;
    p.LHS=lhs;
    string noTerminal="";
    for(int i=0;i<rhs.size();i++){
        if(rhs.at(i)=='\''){
            i++;
            string str= string(1, rhs.at(i++));
            while(rhs.at(i)!='\''){//assume char ' not considered as terminal //assume space between each terminal or not terminal
                str+=string(1, rhs.at(i));
                i++;
                }
               terminals.insert(str);
               p.RHS.push_back(str);
        }else if (rhs.at(i)==' '&&noTerminal!=""){
            p.RHS.push_back(noTerminal);
            noTerminal="";
        }else{
            if(rhs.at(i)!=' ')
                noTerminal+=string(1, rhs.at(i));
            }
        }
        if(noTerminal!=""){
            p.RHS.push_back(noTerminal);
        }
    productions.push_back(p);

    return productions;
}
