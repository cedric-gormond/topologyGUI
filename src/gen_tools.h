//
// Created by Cedric Gormond on 26/01/2019.
//

#ifndef TOPOLOGYGUI_GEN_TOOLS_H
#define TOPOLOGYGUI_GEN_TOOLS_H

#include <stdio.h>
#include <vector>
#include <string>
#include "ctr.hpp"


/*
 * max_gen : returns a vector which contains maximal values
 */
std::vector<int> max_gen(constraint *ctr, int size);

/*
 * getGen : returns the correct gen to write in the output file
 */
std::string setGen(constraint *ctr, int index,int size);

#endif //TOPOLOGYGUI_GEN_TOOLS_H
