//
// Created by Cedric Gormond on 26/01/2019.
//

#include "gen_tools.h"

std::vector<int> max_gen(constraint *ctr, int size){

    std::vector<int> vect;

    int MAX = 0;
    for (int I = 0; I < size; I++) {
        if (stoi(ctr[I].GenX) > stoi(ctr[MAX].GenX)) {
            MAX = I;
        }
    }
    vect.push_back(stoi(ctr[MAX].GenX));

    MAX = 0;
    for (int I = 0; I < size; I++) {
        if (stoi(ctr[I].GenY) > stoi(ctr[MAX].GenY)) {
            MAX = I;
        }
    }
    vect.push_back(stoi(ctr[MAX].GenY));

    MAX = 0;
    for (int I = 0; I < size; I++) {
        if (stoi(ctr[I].GenZ) > stoi(ctr[MAX].GenZ)) {
            MAX = I;
        }
    }
    vect.push_back(stoi(ctr[MAX].GenZ));

    return vect;
}
