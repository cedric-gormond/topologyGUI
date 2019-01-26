//
// Created by Cedric Gormond on 26/01/2019.
//

#include "gen_tools.h"


int max_genX(constraint *ctr, int size) {
    int MAX = 0;
    for (int I = 0; I < size; I++) {
        if (stoi(ctr[I].GenX) > stoi(ctr[MAX].GenX)) {
            MAX = I;
        }
    }
    return MAX;
}

int max_genY(constraint *ctr, int size) {
    int MAX = 0;
    for (int I = 0; I < size; I++) {
        if (stoi(ctr[I].GenY) > stoi(ctr[MAX].GenY)) {
            MAX = I;
        }
    }
    return MAX;
}