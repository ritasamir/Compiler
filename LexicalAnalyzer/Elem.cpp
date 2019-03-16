//
// Created by rita on 16/03/19.
//

#include "Elem.h"


void Elem ::addDestination (int dest) {
    destination.push_back(dest);
}
vector<int> Elem ::getDestination() {
    return destination;
}