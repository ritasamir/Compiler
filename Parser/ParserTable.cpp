#include "ParserTable.h"

ParserTable::ParserTable()
{
    //ctor
}

void ParserTable::fillTable(string nonTerminal, string terminal, vector<string> production)
{
    (parserTable[nonTerminal])[terminal] = production;
}
vector<string> ParserTable::getProduction(string nonTerminal, string terminal)
{
    return (parserTable[nonTerminal])[terminal];
}
bool ParserTable::isEmpty(string nonTerminal, string terminal)
{
    return parserTable[nonTerminal].find(terminal) == parserTable[nonTerminal].end();
}
void ParserTable::printTable()
{
    cout<<endl;
    cout<<"-----------------------Parser Table---------------"<<endl;
    for (map<string, map<string,vector<string>>>  ::iterator it = parserTable.begin(); it != parserTable.end(); ++it)
    {
        map<string,vector<string>> m = it->second;
        cout<<endl;
        cout << "nonTerminal: "<<it->first << endl;
        for ( map<string,vector<string>>  ::iterator it2= m.begin(); it2 != m.end(); ++it2)
        {
            cout << "Input: "<< it2->first << "  ";
            vector<string> v = it2->second;
            cout <<it->first << " -> ";
            for(int i=0 ; i < v.size() ; i++)
            {
                cout<< v[i]<<" ";
            }
            cout << "   "<<endl;
        }
    }

}
