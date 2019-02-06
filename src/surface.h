//
// Created by Cedric Gormond on 06/02/2019.
//

#ifndef TOPOLOGYGUI_SURFACE_H
#define TOPOLOGYGUI_SURFACE_H

#include "ctr.hpp"
#include "gen_tools.h"
#include <cmath>
/*
 * get_surface_hexa : returns surface of a 2D hexagonal topology
 */
int get_surface_hexa(constraint *ctr ,int diagonal, int size);

/*
 * get_surface_2D : returns surface of a 2D topology
 */
int get_surface_2D(constraint *ctr ,int diagonale, int size);

/*
 * get_surface_3D : returns surface of a 3D topology
 */
int get_surface_3D(constraint *ctr ,int d, int size);

/*
 * getDfromS2D :
 */
int getDfromS2D(int gens[3], int coord[4], int surface_2D);

/*
 * getDfromSHexa :
 */
int getDfromSHexa(int gens[3], int coord[4], int surface_hexa);

/*
 * getDfromS3D :
 */
int getDfromS3D(int gens[3], int coord[4], int surface_3D);

#endif //TOPOLOGYGUI_SURFACE_H
