//
//  get_from_ctr.hpp
//  topology
//
//  Created by Cedric Gormond on 25/01/2019.
//  Copyright © 2019 Cedric Gormond. All rights reserved.
//

#ifndef get_from_ctr_hpp
#define get_from_ctr_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "ctr.hpp"
#include "gen_tools.h"

/*
 * getNumberBlocs : Get the number of pblocs (routers)
 */
int getNumberBlocs_from_ctr(std::ifstream& file);

/*
 * getBlocs : Get all the values by using regular expressions
 */
void getBlocs_from_ctr(std::ifstream& file, constraint *ctr);

/*
 * getGen : Get all the values by using regular expressions
 */
void getGen_from_ctr(std::ifstream& file, constraint *ctr);

/*
 * displayBlocs : display blocs in console
 */
void displayBlocs_from_ctr(constraint *ctr, int size);

#endif /* get_from_ctr_hpp */
