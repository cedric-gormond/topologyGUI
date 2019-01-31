//
//  ctr.cpp
//  topology
//
//  Created by Cedric Gormond on 25/01/2019.
//  Copyright © 2019 Cedric Gormond. All rights reserved.
//
#include "ctr.hpp"


// METHODS
void initConstraint(constraint *ctr){
    ctr->name   = " ";
    ctr->X_down = "0";
    ctr->Y_down = "0";
    ctr->X_up   = "0";
    ctr->Y_up   = "0";

    ctr->GenY   = "-1";
    ctr->GenY   = "-1";
    ctr->GenZ   = "-1";

    ctr->CenterX    = 0;
    ctr->CenterY    = 0;

    ctr->width      = 0;
    ctr->heigth     = 0;
}

constraint* CreateConstraint(int gens[2], int coord[4]){
    int size = gens[0] * gens[1];
    constraint* ctr_temp = new constraint[size];

    ctr_temp[0].X_down  = std::to_string(coord[0]);
    ctr_temp[0].Y_down  = std::to_string(coord[1]);
    ctr_temp[0].X_up    = std::to_string(coord[2]);
    ctr_temp[0].Y_up    = std::to_string(coord[3]);
    ctr_temp[0].GenX    = std::to_string(gens[0]);
    ctr_temp[0].GenY    = std::to_string(gens[1]);


    return ctr_temp;
}
