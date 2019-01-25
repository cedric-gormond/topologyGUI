#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>

#include "block.h"
#include "blockContainer.h"
#include "lineContainer.h"
#include "ctr.hpp"
#include "get_from_ctr.hpp"
#include "file_tools.hpp"
#include "topology.hpp"
#include "create_output_file.hpp"

using namespace std;

int main() {
    // ---- FILE INPUT ---
    //Please past the path's file
    string file_path = "/Users/cedricgormond/Desktop/cf_xcode/topology/topology/fichier_contrainte_3D.txt"; //FULL PATH

    ifstream file(file_path.c_str(), ios::in);
    string input_filename = getFilename(file_path); // get the input file name WITHOUT extension

    // Check
    if (file) {
        cout << " * File found :" << endl;
        SplitFilename(file_path);
    }
    else {
        cout << " * Cannot find  :" <<endl;
        SplitFilename(file_path);

        cout << endl << " * The constraint file must be named : fichier_contrainte_3D.txt \n \n";
        cout << " * Exit" <<endl<<endl;

        exit (EXIT_FAILURE);
    }

    // ----- READ INPUT FILE AND EXTRACT BLOCS  ------

    //Get number of routers (blocs)
    cout << endl;
    cout << " * Searching for pblocs " << endl;
    int nb_pblocs = getNumberBlocs_from_ctr(file);
    cout << " * " <<nb_pblocs << " pblocs found \n";
    cout << endl;
    file.close();

    //Constraint values
    constraint *CONTRAINT = new constraint[nb_pblocs];
    initConstraint(CONTRAINT);

    // Get all the routers (pblocs)
    file.open(file_path);
    getBlocs_from_ctr(file, CONTRAINT);
    file.close();

    // Get all the generation of each (pblocs)
    file.open(file_path);
    getGen_from_ctr(file, CONTRAINT);
    file.close();

    // Set the center of all blocs
    dimensions_of_bloc(CONTRAINT, nb_pblocs);

    // Display blocs in console
    displayBlocs_from_ctr(CONTRAINT, nb_pblocs);

    sf::RenderWindow window(sf::VideoMode(1200, 1200), "SFML works!");
    window.setFramerateLimit(30);
    window.setVerticalSyncEnabled(false);

    // create container with 5 blocks in it, starting at pos 10/10
    // this container will be drawn using ContainerOfBlocks' void drawContainer(sf::RenderWindow &window)
    ContainerOfBlocks testBlocks(5, sf::Vector2f(10.0f, 10.0f));

    // create another container, starting at pos 10/50
    // this one will be drawn using sf::Drawable's function to draw
    ContainerOfBlocks testBlocks2(5, sf::Vector2f(10.0f, 50.0f));

    // create  line container, starting at pos 10/50
    // this one will be drawn using sf::Drawable's function to draw
    ContainerOfLines testlines(2, sf::Vector2f(34.0f, 100.0f));

    while (window.isOpen()) {
        sf::Event evt;
        while (window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        testBlocks.drawContainer(window);
        window.draw(testBlocks2);
        window.display();
    }

    return 0;
}