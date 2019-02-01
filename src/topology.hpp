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
#include "gen_tools.h"

/*
 * center_of_bloc :
 */
void dimensions_of_bloc(constraint *ctr ,int size);

/*
 * resize_dimensions :
 */
void resize_dimensions(constraint *ctr ,int size, int vec2i[2]);

/*
 * distance_between_blocs :
 */
int distance_between_blocs(constraint *ctr, int bloc1, int bloc2);

/*
 *
 */
int get_surface_hexa(constraint *ctr ,int radius);

/*
 *
 */
int get_surface_2D(constraint *ctr ,int diagonale, int size);

/*
 *
 */
int get_surface_3D(constraint *ctr ,int d, int size);

/*
 * set_distance_from_bloc1 :
 */
constraint* set_2D_from_bloc1(constraint *ctr ,int distance, int size);

/*
 * set_hexa:
 */
constraint* set_hexa(constraint *ctr ,int radius, int size);

/*
 * set_3D:
 */
constraint* set_3D(constraint *ctr ,int distance, int size);

#endif /* topology_hpp */
