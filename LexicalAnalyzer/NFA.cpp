#include "NFA.h"
#include <iostream>
#include <string.h>
#include "global.h"
#include "State.h"
#include <stack>
NFA::NFA()
{
    //ctor
}

NFA::~NFA()
{
    //dtor
}
vector<State> startStates;
State createConcatenatedStates(string kw,State s,string TokenType)
{
    if(kw.size()==0)
    {
        s.setAcceptanceSate(true);
        s.setTokenType(TokenType);
        return s;
    }
    s.add_input(string(1,kw.at(0)));
    s.setAcceptanceSate(false);
    kw.erase(0,1);
    State next;
    s.add_output(createConcatenatedStates(kw,next,TokenType));
    return s;
}
void createKeywordsStates()
{
    for (int i = 0; i < keywords.size(); ++i)
    {
        State start;
        start = createConcatenatedStates(keywords.at(i),start,keywords.at(i));
        startStates.push_back(start);
    }
}
void createPunctuationsStates(){
    for (int i = 0; i < punctuations.size(); ++i)
    {
        State start;
        start = createConcatenatedStates(punctuations.at(i),start,"punctuation");
        startStates.push_back(start);
    }
}
void NFA::functionTest()
{
    createKeywordsStates();
    createPunctuationsStates();
    for (auto & element : startStates)
    {
        auto & s = element;

        while(!s.isAcceptanceState())
        {
            cout <<"transition = "<< s.get_input().at(0)<<endl;
            s = s.get_output().at(0);
        }
        cout <<"Token type " <<s.getTokenType()<<endl;
    }

}
