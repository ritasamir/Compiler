#ifndef STATES_H
#define STATES_H

#include<bits/stdc++.h>
using namespace std;
class States
{
    public:
        States();
        virtual ~States();
        void add_input(string input);
        void add_output(States state);
        vector<string> get_input();
        vector<States> get_output();
        void setAcceptanceSate(bool status);
        bool isAcceptanceState();
        void setTokenType(string type);
        string getTokenType();
    private:
        vector<string> inputs;
        vector<States> outputs;
        bool acceptanceState;
        string tokenType;
};

#endif // STATES_H
