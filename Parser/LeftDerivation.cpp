#include "LeftDerivation.h"
#include <stack>


void LeftDerivation::derive(map<string, map<string,vector<string>>> parserTable, string start, vector<token> input)
{
    ofstream outfile;
    outfile.open("derivationOutput.txt",  std::ios_base::app);
    input.push_back({"$", "$"});
    stack<string> stack;
    vector<string> print_vector;
    stack.push("$");
    stack.push(start);
    print_vector.emplace_back("$");
    print_vector.push_back(start);
    outfile << "$ ";
    outfile << start << endl;
    int i = 0;
    auto print_vector_it  = ++print_vector.begin();
    while(!stack.empty())
    {
        if(i >= input.size())
        {
            outfile << "error: input ended but not accepted" << endl;
            break;
        }
        if(!is_terminal(input[i].TokenType))
        {
            outfile << "error: undefined terminal, discard input" << endl;
            break;
        }
        else
        {
            if(is_nonTerminal(stack.top()))
            {
                vector<string> prod = (parserTable[stack.top()])[input[i].TokenType];

                if(prod.at(0) == "error")
                {
                    outfile << "error: no production, discard input" << endl;
                    //remove input
                    i++;
                }
                else if(prod.at(0) == "synch")
                {
                    outfile << "synch cell the letter is in the follow set" << endl;
                    stack.pop();
                    print_vector.erase(print_vector_it);
                    for (auto j = print_vector.begin(); j != print_vector.end(); j++)
                    {
                        outfile << *j << " ";
                    }
                }
                else if(prod.at(0) == "\\L")
                {
                    stack.pop();
                    while(!is_nonTerminal(*print_vector_it))
                    {
                        print_vector_it++;
                    }
                    print_vector.erase(print_vector_it);
                    for (auto j = print_vector.begin(); j != print_vector.end(); j++)
                    {
                        outfile << *j << " ";
                    }
                    outfile << "\n";
                }
                else
                {
                    stack.pop();
                    print_vector.erase(print_vector_it);
                    //   cout << "print after erase "<<*print_vector_it << endl;
                    for (auto j = prod.rbegin(); j != prod.rend(); j++)
                    {
                        stack.push(*j);
                    }
                    print_vector.insert(print_vector_it, prod.begin(),prod.end());
                    string first_element = *prod.begin();
                    for(auto j = print_vector.begin();j!= print_vector.end();j++){
                        if((*j) == first_element){
                            print_vector_it = j;

                        }
                    }
                    //print_vector_it = std::find(print_vector.rbegin(),print_vector.rend(),*prod.begin());
                   // cout << "print after insert "<<*print_vector_it << endl;

                    for (auto j = print_vector.begin(); j != print_vector.end(); j++)
                    {
                        outfile << *j << " ";
                    }
                    outfile << "\n";
                }
            }
            else
            {
                if(is_terminal(stack.top()))
                {
                    if(stack.top() == input[i].TokenType)
                    {
                        i++;
                        print_vector_it++;
                      //cout << "print it "<<*print_vector_it << endl;

                    }
                    else
                    {
                        outfile << "error: non matching terminals, missing terminal " << stack.top() << endl;
                        print_vector.erase(print_vector_it);
                        for (auto j = print_vector.begin(); j != print_vector.end(); j++)
                        {
                            outfile << *j << " ";
                        }
                        outfile << "\n";
                    }
                    stack.pop();
                }
            }
        }

    }
    if(stack.empty() && i < input.size())
    {
        outfile << "error: stack ended but input didn't" << endl;
    } else {
        outfile << "accept\n";
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
    return terminals.find(token)!= terminals.end() || token == "$";
}
bool LeftDerivation::is_nonTerminal(string token)
{
    return nonTerminals.find(token)!= nonTerminals.end();
}
