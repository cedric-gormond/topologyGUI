//
//  get_from_txt.cpp
//  topology
//
//  Created by Cedric Gormond on 25/01/2019.
//  Copyright © 2019 Cedric Gormond. All rights reserved.
//

#include "get_from_txt.hpp"

using namespace std;

int getNumberBlocs_from_text(ifstream& file){
    string line;
    int nb_blocs = 0;
    
    while(getline(file,line)){
        size_t found = line.find("pblock"); //Search lines with pblock
        if (found!= string::npos){
            nb_blocs++;
        }
    }
    return nb_blocs;
}

void getBlocs_from_text(std::ifstream& file, constraint *ctr)
{
    std::string line; //a line of the constraints file
    int i = 0;
    
    while (getline(file, line)) {
        size_t found = line.find("X0"); //Shearch lines with X or Y
        std::smatch matches; //matches
        std::string raw = (R"(X\d*\s?\:\s*(\d{1,4})\s*Y\d*\s?\:\s*(\d{1,4})\s*\|\|\s*X\d{1,}\s?\:\s*(\d{1,3})\s*Y\d{1,}\s?\:\s*(\d{1,3}))"); //regular expression
        std::regex rgx;
        
        if (found!= string::npos){
            try
            {
                rgx = std::regex(raw);
            }
            catch (std::regex_error& e) //if it doesn't work
            {
                std::cout << "Error" << std::endl;
                std::cout << e.what() << std::endl;
            }
            
            if (std::regex_search(line, matches, rgx))
            {
                // matches[0] is the complete match
                ctr[i].X_down = matches[2].str(); //Group 2 : Get X0
                ctr[i].Y_down = matches[3].str(); //Group 3 : Get Y0
                ctr[i].X_up = matches[4].str(); //Group 4 : Get X1
                ctr[i].Y_up = matches[5].str(); //Group 5 : Get Y1
            }
            else{
                std::cout << "pblocks not found\n";
            }
            i++;
        }
    }
}

void getGen_from_text(std::ifstream& file, constraint *ctr)
{
    std::string line; //a line of the constraints file
    int i = 0;
    
    
    while (getline(file, line)) {
        size_t found = line.find("pblock"); //Shearch lines with add_cells_to_pblock
        std::smatch matches; //matches
        std::string raw3D = (R"(pblock_(\d*)\s*\s*\(GEN_Z\[(\d{1,2})\]\s?GEN_Y\[(\d{1,2})\]\s?GEN_X\[(\d{1,2})\]\))"); //regular expression 3D
        std::string raw2D = (R"(pblock_(\d*)\s*\s*\(GEN_Y\[(\d{1,2})\]\s?GEN_X\[(\d{1,2})\]\))"); //regular expression 2D
        std::regex rgx3D, rgx2D;
        
        if (found!= string::npos){
            try
            {
                rgx3D = std::regex(raw3D);
                rgx2D = std::regex(raw2D);
            }
            catch (std::regex_error& e) //if it doesn't work
            {
                std::cout << "Error" << std::endl;
                std::cout << e.what() << std::endl;
            }
            
            if (std::regex_search(line, matches, rgx3D))
            {
                // matches[0] is the complete match
                ctr[i].name = matches[1].str(); //Group 1 : get name
                ctr[i].GenZ = matches[2].str(); //Group 2 : Gen Z
                ctr[i].GenY = matches[3].str(); //Group 3 : Gen Y
                ctr[i].GenX = matches[4].str(); //Group 4 : Gen X
            }
            else{
                if (std::regex_search(line, matches, rgx2D))
                {
                    // matches[0] is the complete match
                    ctr[i].name = matches[1].str(); //Group 1 : get name
                    ctr[i].GenY = matches[2].str(); //Group 2 : Gen Y
                    ctr[i].GenX = matches[3].str(); //Group 3 : Gen X
                }
                //std::cout << "gen not found\n";
            }
            i++;
        }
    }
}

std::string setGen(constraint *ctr, int index, int size)
{
    /*
     * GenZ doesn't matter
     */
    
    string correct_gen;
    string str_X, str_Y;
    int max_X, max_Y;
    
    
    /*
     * Different method because it didn't work (i don't know why)
     */
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
    
    // Determines the first expression which depends on Y
    if(stoi(ctr[index].GenY) == 0){
        str_Y = ".GEN_ROUTER_BOTTOM";
    }
    
    if (stoi(ctr[index].GenY) == max_Y) {
        str_Y = ".GEN_ROUTER_TOP";
    }
    
    if ((stoi(ctr[index].GenY) != 0) && (stoi(ctr[index].GenY) != max_Y)) {
        str_Y = ".GEN_ROUTER_MIDDLE";
    }
    
    // Determines the second expression which depends on X
    if(stoi(ctr[index].GenX) == 0){
        str_X = str_Y + "_LEFT";
    }
    
    if (stoi(ctr[index].GenX) == max_X) {
        str_X = str_Y + "_RIGHT";
    }
    
    if ((stoi(ctr[index].GenX) != 0) && (stoi(ctr[index].GenX) != max_X)) {
        str_X = str_Y + "_CENTRAL";
    }
    
    //final gen
    correct_gen = str_Y + str_X;
    
    return correct_gen;
}

void createOutputFile(std::ofstream& file_output, string path,constraint *ctr, int size)
{
    cout << " * Wrinting in file " << endl << endl;
    
    //first line which create clock and its features
    file_output << "create_clock -period 5.000 -name clk -waveform {0.000 2.500} [get_ports Clock]" << endl;
    file_output << endl << endl;
    
    // string which contains the right expression of .GEN
    string set_gen;
    
    //Creation of a bloc
    for (int i=0; i < size; i++) {
        file_output << "create_pblock pblock_"<< ctr[i].name << endl;
        
        set_gen = setGen(ctr,i,size);
        
        file_output << "add_cells_to_pblock [get_pblocks pblock_"<< ctr[i].name <<"] [get_cells -quiet [list {GEN_Z["<< ctr[i].GenZ <<"].GEN_Y["<< ctr[i].GenY <<"].GEN_X["<< ctr[i].GenX <<"]"<< set_gen <<".Router}]]" << endl;
        
        file_output << "resize_pblock [get_pblocks pblock_"<< ctr[i].name <<"] -add {SLICE_X"<< ctr[i].X_down <<"Y"<< ctr[i].Y_down <<":SLICE_X"<< ctr[i].X_up <<"Y"<< ctr[i].Y_up <<"}" << endl;
        
        file_output << endl;
    }
}
