//
// Created by rita on 16/03/19.
//

#ifndef LEXICALANALYZER_ELEM_H
#define LEXICALANALYZER_ELEM_H
#include <vector>
using namespace std ;

class Elem {

private:
    vector<int> destination;

public:
    void addDestination (int);
    vector<int> getDestination(void);

};


#endif //LEXICALANALYZER_ELEM_H
