//
//  topology.hpp
//  topology
//
//  Created by Cedric Gormond on 25/01/2019.
//  Copyright © 2019 Cedric Gormond. All rights reserved.
//

#ifndef topology_hpp
#define topology_hpp

#include <stdio.h>
#include <math.h>
#include "ctr.hpp"

/*
 * center_of_bloc :
 */
void dimensions_of_bloc(constraint *ctr ,int size);

/*
 * distance_between_blocs :
 */
int distance_between_blocs(constraint *ctr, int bloc1, int bloc2,int size);

/*
 * set_distance_from_bloc1 : 
 */
constraint* resize_2D_from_bloc1(constraint *ctr ,int distance, int size);

#endif /* topology_hpp */
