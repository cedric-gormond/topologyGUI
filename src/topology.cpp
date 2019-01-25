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
        ctr[i].width = abs(stoi(ctr[i].X_up)-stoi(ctr[i].X_down));
        ctr[i].heigth = abs(stoi(ctr[i].Y_up)-stoi(ctr[i].Y_down));
        
        //center
        ctr[i].CenterX = abs(stoi(ctr[i].X_up)+stoi(ctr[i].X_down)) / 2;
        ctr[i].CenterY = abs(stoi(ctr[i].Y_up)+stoi(ctr[i].Y_down)) / 2;
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

constraint* resize_2D_from_bloc1(constraint *ctr ,int distance, int size)
{
    constraint *ctr_resize = new constraint[size];
    //initConstraint(ctr_resize);
    
    //OR ! *ctr_resize = ctr;
    std::copy(ctr, ctr + size, ctr_resize); // copy the data from ctr into ctr_resize
    
    int max_X, max_Y;
    
    // We need to know how many genX we have
    int MAX=0;
    for (int I=0; I<size; I++){
        if(stoi(ctr[I].GenX)>stoi(ctr[MAX].GenX)){
            MAX=I;
        }
    }
    
    max_X = stoi(ctr[MAX].GenX);
    
    // We need to know how many genY we have
    max_Y = stoi(ctr[0].GenY);
    for (int i = 1; i < size - 1; i++) {
        if (stoi(ctr[i+1].GenY) > max_Y) {
            max_Y = stoi(ctr[i].GenY); //stoi : string to int
        }
    }
    
    //max_X = 2;
    
    // recalculate every center
    // assumes that pbloc is bottom_left
    int i, j, pbloc;
    
    i = j = pbloc = 0;
    while (i<=max_Y) {
        j=0;
        while (j<=max_X) {
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
        ctr_resize[i].X_up = std::to_string(X1_temp);
        
        // X1 Y1
        Y0_temp = ctr_resize[i].CenterY - (ctr_resize[i].heigth/2);
        Y1_temp = ctr_resize[i].CenterY + (ctr_resize[i].heigth/2);
        
        ctr_resize[i].Y_down = std::to_string(Y0_temp);
        ctr_resize[i].Y_up = std::to_string(Y1_temp);
    }
    
    return ctr_resize;
}
