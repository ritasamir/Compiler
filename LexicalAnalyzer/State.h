#ifndef STATE_H
#define STATE_H

#include<bits/stdc++.h>
using namespace std;
class State
{
    public:
        State();
        virtual ~State();
        void add_input(string input);
        void add_output(State state);
        vector<string> get_input();
        vector<State> get_output();
        void setAcceptanceSate(bool status);
        bool isAcceptanceState();
        void setTokenType(string type);
        string getTokenType();
    private:
        vector<string> inputs;
        vector<State> outputs;
        bool acceptanceState;
        string tokenType;
};

#endif // STATES_H
