//
// Created by rita on 12/04/19.
//

#include "ParsingCFG.h"
#include <regex>
#include <iostream>
#include <fstream>

using namespace std;

vector<Production> ParsingCFG::getProductions(string fileName){

    stack<string> lines = parseFile(fileName);
    vector<Production> productions;
    stack<string> s=lines;
    string rhs;
    string lhs;

    while (!s.empty())
    {
        string line = s.top();
        s.pop();
        rhs="";
        lhs="";
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
    startNonTerminal=lhs;

    productions = eliminateLeftReccursion(productions);
    productions = eliminateLeftFactoring(productions);
    return productions;
}

stack<string> ParsingCFG::parseFile(string fileName) {
    ifstream file(fileName);
    string line;
    stack<string> lines;
    while (getline(file, line)) {
        if ( regex_match(line, regex("^#(\\s)*([a-zA-Z0-9_]+)(\\s)*=(\\s)*[\\x00-\\x7B}~]+")) ) {
            lines.push(line);

        }else if(regex_match(line, regex("^#(\\s)*([a-zA-Z0-9_]+)(\\s)*=(\\s)*[\\x00-\\x7F]+"))){
            lines.push(line);
        }else if(regex_match(line, regex("^[\\x00-\\x7F]+"))) {
            string temp = lines.top();
            lines.pop();
            temp+=line;
            lines.push(temp);

        }else{
            cout<<line<<" Error : No match"<<endl;
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

void ParsingCFG::printProductions(vector<Production> productions) {
    cout<<"production : "<<endl;
    for(Production l :  productions){
        cout<<l.LHS<<" ---> ";
        int i=1;
        for(string s:l.RHS){
            cout<<i<<"-"<<s<<" ";
            i++;
        }
        cout<<endl;
    }
    cout<<"terminals : "<<endl;
    for(string s :terminals){
        cout<<s<<"  ";
    }
    cout<<endl;
    cout<<"Starting NonTerminal : "<<startNonTerminal<<endl;
}

vector<Production> ParsingCFG::eliminateLeftReccursion(vector<Production> productions) {
    string lhsRec="";
    string newNonTerminal="";
    vector<Production> temp;
    for(vector<Production>::iterator i = productions.begin();i !=productions.end(); ++i){
        if((*i).LHS == (*i).RHS.at(0)){ // there exist left recursion
            lhsRec=(*i).LHS;
            (*i).LHS+="\\";
            newNonTerminal=(*i).LHS;
            (*i).RHS.erase((*i).RHS.begin());
            (*i).RHS.push_back((*i).LHS);
            Production newP;
            newP.LHS=newNonTerminal;
            newP.RHS.push_back("\\L");
            temp.push_back(newP);
            eliminateLeftReccursionHelper(&productions, i, newNonTerminal, lhsRec);
        }
    }
    for(Production p : temp){
        productions.push_back(p);
    }

    return productions;
}

void ParsingCFG::eliminateLeftReccursionHelper(vector<Production> *productions, vector<Production>::iterator iterator, string newNonTerminal,
                                          string lhsRec) {
    for(vector<Production>::iterator i = productions->begin();i !=productions->end(); ++i){
        if((*i).LHS==lhsRec&& i!= iterator){
            (*i).RHS.push_back(newNonTerminal);
        }
    }
}

vector<Production> ParsingCFG::eliminateLeftFactoring(vector<Production> productions) {
    bool leftFactoring;
    vector<Production> temp;
    for(vector<Production>::iterator i = productions.begin();i !=productions.end(); ++i){
        leftFactoring=false;
        for(vector<Production>::iterator j = productions.begin();j !=productions.end(); ++j){
            if((*i).LHS==(*j).LHS && (*i).RHS.at(0)==(*j).RHS.at(0)&& j!= i){

                leftFactoring=true;
                (*j).LHS+="\\";
                if((*j).RHS.size()>1)
                    (*j).RHS.erase((*j).RHS.begin());
                else
                    (*j).RHS.at(0)="\\L";


            }
        }
        if(leftFactoring){
            Production p;
            p.LHS= (*i).LHS;
            p.RHS.push_back((*i).RHS.at(0));
            p.RHS.push_back((*i).LHS+"\\");
            temp.push_back(p);
            (*i).LHS+="\\";
            if((*i).RHS.size()>1)
                (*i).RHS.erase((*i).RHS.begin());
            else
                (*i).RHS.at(0)="\\L";
        }
    }

    for(Production p : temp){
        productions.push_back(p);
    }
    return productions;
}
