//
//  topology.cpp
//  topology
//
//  Created by Cedric Gormond on 25/01/2019.
//  Copyright © 2019 Cedric Gormond. All rights reserved.
//

#include "topology.hpp"

void dimensions_of_bloc(constraint *ctr ,int size){
    for (int i=0; i<size; i++) {

        //dimensions
        ctr[i].width    = abs(stoi(ctr[i].X_up)-stoi(ctr[i].X_down));
        ctr[i].heigth   = abs(stoi(ctr[i].Y_up)-stoi(ctr[i].Y_down));

        //center
        ctr[i].CenterX  = abs(stoi(ctr[i].X_up)+stoi(ctr[i].X_down)) / 2;
        ctr[i].CenterY  = abs(stoi(ctr[i].Y_up)+stoi(ctr[i].Y_down)) / 2;
    }
}

void resize_dimensions(constraint *ctr ,int size, int vec2i[2]){
    for (int i=0; i<size; i++) {
        ctr[i].width    = vec2i[0];
        ctr[i].heigth   = vec2i[1];
    }
}

int distance_between_blocs(constraint *ctr, int bloc1, int bloc2,int size)
{
    int distance = 0;
    int XC_1,XC_0,YC_1,YC_0;

    XC_0 = ctr[bloc1].CenterX;
    YC_0 = ctr[bloc1].CenterY;

    XC_1 = ctr[bloc2].CenterX;
    YC_1 = ctr[bloc2].CenterX;

    if ((bloc1 != bloc2)) {
        //distance = sqrt(pow((XC_1 - XC_0),2) + pow((YC_1 - YC_0),2)); //Euclidean space
        distance = abs(XC_1 - XC_0) + abs(YC_1 - YC_0); //Mahnattan
    }
    return distance;
}

int get_surface_hexa(constraint *ctr ,int radius, int size){
    //we want the surface area of the hexa form. However, we will just calculate the surface of a square
    int total_width     = round(2*(radius + ctr[0].width/2));
    int total_heigth    = round(2*(radius + ctr[0].heigth/2));
    return (total_width * total_heigth);
}

int get_surface_2D(constraint *ctr ,int distance, int size){
    std::vector<int> gen_max(3);
    gen_max = max_gen(ctr, size);

    int total_width     = round(gen_max[0]*distance + ctr[0].width/2);
    int total_heigth    = round(gen_max[1]*distance + ctr[0].heigth/2);
    return (total_width * total_heigth);
}

constraint* set_2D_from_bloc1(constraint *ctr ,int distance, int size)
{
    constraint *ctr_resize = new constraint[size];
    //initConstraint(ctr_resize);

    ctr_resize = ctr;// copy the data from ctr into ctr_resize


    std::vector<int> gen_max(3);
    gen_max = max_gen(ctr, size);

    // recalculate every center and assumes that pblock is bottom_left
    int i, j, pbloc;

    i = j = pbloc = 0;
    while (i<=gen_max[1]) {
        j=0;
        while (j<=gen_max[0]) {
            pbloc++;
            ctr_resize[pbloc].CenterX = ctr_resize[pbloc-1].CenterX + distance;
            ctr_resize[pbloc].CenterY = ctr_resize[pbloc-1].CenterY;
            j++;
        }
        ctr_resize[pbloc].CenterX = ctr_resize[0].CenterX ;
        ctr_resize[pbloc].CenterY = ctr_resize[pbloc].CenterY + distance;
        i++;
    }

    // recalculate every X0Y0 and X1Y1
    int X0_temp, Y0_temp;
    int X1_temp, Y1_temp;

    for (int i=0; i<size; i++) {
        // X0 Y0
        X0_temp = ctr_resize[i].CenterX - (ctr_resize[i].width/2);
        X1_temp = ctr_resize[i].CenterX + (ctr_resize[i].width/2);

        ctr_resize[i].X_down = std::to_string(X0_temp);
        ctr_resize[i].X_up   = std::to_string(X1_temp);

        // X1 Y1
        Y0_temp = ctr_resize[i].CenterY - (ctr_resize[i].heigth/2);
        Y1_temp = ctr_resize[i].CenterY + (ctr_resize[i].heigth/2);

        ctr_resize[i].Y_down = std::to_string(Y0_temp);
        ctr_resize[i].Y_up   = std::to_string(Y1_temp);
    }

    return ctr_resize;
}

constraint* set_hexa(constraint *ctr, int radius, int size)
{
    constraint *ctr_resize = new constraint[size];

    std::copy(ctr, ctr + size, ctr_resize); // copy the data from ctr into ctr_resize

    // we move the first block in the middle
    ctr_resize[0].CenterX = 100;
    ctr_resize[0].CenterY = 100;

    //number of extremities around the center (assumes that pbloc is the center)
    int nb_blocs = size - 1;

    //angle
    int angle = round((360/nb_blocs));

    //calculate positions
    int pbloc = 1;
    for (int i=0; i<nb_blocs; i++) {
        ctr_resize[pbloc].CenterX =  (int)(radius*cos(angle*i*(3.14159/180)) + ctr_resize[0].CenterX); // the origine is (100,100)
        ctr_resize[pbloc].CenterY =  (int)(radius*sin(angle*i*(3.14159/180)) + ctr_resize[0].CenterY);
        pbloc++;
    }

    // recalculate every X0Y0 and X1Y1
    int X0_temp, Y0_temp;
    int X1_temp, Y1_temp;

    for (int i=0; i<size; i++) {
        // X0 Y0
        X0_temp = ctr_resize[i].CenterX - (ctr_resize[i].width/2);
        X1_temp = ctr_resize[i].CenterX + (ctr_resize[i].width/2);

        ctr_resize[i].X_down = std::to_string(X0_temp);
        ctr_resize[i].X_up   = std::to_string(X1_temp);

        // X1 Y1
        Y0_temp = ctr_resize[i].CenterY - (ctr_resize[i].heigth/2);
        Y1_temp = ctr_resize[i].CenterY + (ctr_resize[i].heigth/2);

        ctr_resize[i].Y_down = std::to_string(Y0_temp);
        ctr_resize[i].Y_up   = std::to_string(Y1_temp);
    }

    return ctr_resize;
}

constraint* set_3D(constraint *ctr ,int distance, int size){
    constraint *ctr_resize = new constraint[size];

    std::copy(ctr, ctr + size, ctr_resize); // copy the data from ctr into ctr_resize

    std::vector<int> gen_max(3);
    gen_max = max_gen(ctr, size);

    // recalculate every center and assumes that pblock is bottom_left
    int i, j, pbloc;

    i = j = pbloc = 0;
    while (i<=gen_max[1]) {
        j=0;
        while (j<=gen_max[0]) {
            pbloc++;
            ctr_resize[pbloc].CenterX = ctr_resize[pbloc-1].CenterX + distance;
            ctr_resize[pbloc].CenterY = ctr_resize[pbloc-1].CenterY;
            j++;
        }
        ctr_resize[pbloc].CenterX = ctr_resize[0].CenterX ;
        ctr_resize[pbloc].CenterY = ctr_resize[pbloc].CenterY + distance;
        i++;
    }

    // recalculate every X0Y0 and X1Y1
    int X0_temp, Y0_temp;
    int X1_temp, Y1_temp;

    for (int i=0; i<size; i++) {
        // X0 Y0
        X0_temp = ctr_resize[i].CenterX - (ctr_resize[i].width/2);
        X1_temp = ctr_resize[i].CenterX + (ctr_resize[i].width/2);

        ctr_resize[i].X_down = std::to_string(X0_temp);
        ctr_resize[i].X_up   = std::to_string(X1_temp);

        // X1 Y1
        Y0_temp = ctr_resize[i].CenterY - (ctr_resize[i].heigth/2);
        Y1_temp = ctr_resize[i].CenterY + (ctr_resize[i].heigth/2);

        ctr_resize[i].Y_down = std::to_string(Y0_temp);
        ctr_resize[i].Y_up   = std::to_string(Y1_temp);
    }

    return ctr_resize;
}
