#include "LeftDerivation.h"
#include <stack>


void LeftDerivation::derive(map<string, map<string,vector<string>>> parserTable, string start)
{
    ofstream outfile;
    outfile.open("C:/Users/omid/Desktop/Parser/derivationOutput.txt", ios::out);
    string input = "ceadb";
    bool terminal;
    input.push_back('$');
    stack<string> v;
    vector<string> print_vector;
    v.push("$");
    v.push(start);
    print_vector.push_back("$");
    print_vector.push_back(start);
    outfile << "$";
    outfile << start << endl;
    int i = 0;
    auto print_vector_it  = ++print_vector.begin();
    while(!v.empty())
    {
        if(i >= input.size())
        {
            outfile << "error: input ended but stack didn't" << endl;
            break;
        }
        if(!is_terminal(string(1,input[i])))
        {
            outfile << "undefined terminal" << endl;
            break;
        }
        else
        {
            if(is_nonTerminal(v.top()))
            {
                vector<string> prod = (parserTable[v.top()])[string(1,input[i])];

                if(prod.at(0) == "error")
                {
                    outfile << "error: no production, discard input" << endl;
                    //remove input
                    i++;
                }
                else if(prod.at(0) == "sync")
                {
                    outfile << "error: sync cell the letter is in the follow set";
                    v.pop();
                    print_vector.erase(print_vector_it);
                    for (auto j = print_vector.begin(); j != print_vector.end(); j++)
                    {
                        outfile << *j;
                    }
                }
                else if(prod.at(0) == "\\L")
                {
                    v.pop();

                    while(!is_nonTerminal(*print_vector_it))
                    {
                        print_vector_it++;
                    }
                    print_vector.erase(print_vector_it);
                    for (auto j = print_vector.begin(); j != print_vector.end(); j++)
                    {
                        outfile << *j;
                    }
                    outfile << "\n";
                }
                else
                {
                    v.pop();
                    //   cout << "print before erase "<<*print_vector_it << endl;
                    print_vector.erase(print_vector_it);
                    //   cout << "print after erase "<<*print_vector_it << endl;
                    for (auto j = prod.rbegin(); j != prod.rend(); j++)
                    {
                        v.push(*j);
                    }
                    print_vector.insert(print_vector_it, prod.begin(),prod.end());
                    print_vector_it = find(print_vector.begin(),print_vector.end(),*prod.begin());
                    //   cout << "print after insert "<<*print_vector_it << endl;
                    for (auto j = print_vector.begin(); j != print_vector.end(); j++)
                    {
                        outfile << *j;
                    }
                    outfile << "\n";
                    //  cout << "\n";
                }
            }
            else
            {
                if(is_terminal(v.top()))
                {
                    if(v.top() == string(1,input[i]))
                    {
                        i++;
                        //     cout << "match " << v.top() << endl;
                        print_vector_it++;
                        //  cout << "print it "<<*print_vector_it << endl;
                    }
                    else
                    {
                        outfile << "error: non matching terminals, missing terminal" << endl;
                        print_vector.erase(print_vector_it);
                        for (auto j = print_vector.begin(); j != print_vector.end(); j++)
                        {
                            outfile << *j;
                        }
                        outfile << "\n";
                    }
                    v.pop();
                }
            }
        }

    }
    if(v.empty() && i < input.size())
    {
        outfile << "error: stack ended but input didn't" << endl;
    }
    outfile.close();

}
void LeftDerivation::set_terminals(set<string> input)
{
    terminals = input;
}
void LeftDerivation::set_nonTerminals(set<string> input)
{
    nonTerminals = input;
}
bool LeftDerivation::is_terminal(string token)
{
    return terminals.find(token)!= terminals.end();
}
bool LeftDerivation::is_nonTerminal(string token)
{
    return nonTerminals.find(token)!= nonTerminals.end();
}
