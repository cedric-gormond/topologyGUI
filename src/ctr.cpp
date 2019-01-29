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
