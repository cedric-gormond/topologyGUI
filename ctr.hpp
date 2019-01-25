//
//  ctr.hpp
//  topology
//
//  Created by Cedric Gormond on 25/01/2019.
//  Copyright © 2019 Cedric Gormond. All rights reserved.
//

#ifndef ctr_hpp
#define ctr_hpp

#include <iostream>
#include <string>


// STRUCT
typedef struct {
    std::string name;
    std::string X_up,Y_up,X_down,Y_down;
    std::string GenX = "-1";
    std::string GenY = "-1";
    std::string GenZ = "-1";
    
    int CenterX, CenterY, width, heigth;
}constraint;

/*
 * initConstraint : Initialisation of constraint pointer
 */
void initConstraint(constraint *ctr);

#endif /* ctr_hpp */
