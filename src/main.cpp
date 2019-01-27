#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

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
    string file_path = "/Users/cedricgormond/Desktop/cf_xcode/topology/topology/fichier_contrainte_2D.txt"; //FULL PATH

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

    //Resize constraint
    constraint *CONTRAINT_RESIZE = resize_2D_from_bloc1(CONTRAINT, 100, nb_pblocs);

    // ------ GUI --------
    sf::RenderWindow window(sf::VideoMode(600, 600), "topologyGUI");
    window.setFramerateLimit(30);
    //window.setVerticalSyncEnabled(false);

    //ImGui init
    ImGui::SFML::Init(window);

    // create container with 5 blocks in it, starting at pos 10/10
    // this container will be drawn using ContainerOfBlocks' void drawContainer(sf::RenderWindow &window)
    ContainerOfBlocks testBlocks(nb_pblocs, sf::Vector2f(100.0f, 100.0f),CONTRAINT_RESIZE);

    // create  line container, starting at pos 10/50
    // this one will be drawn using sf::Drawable's function to draw
    ContainerOfLines testlines(nb_pblocs, sf::Vector2f(150.0f, 100.0f), CONTRAINT_RESIZE,100);

    int distance;

    sf::Color bgColor;

    float color[3] = { 0.f, 0.f, 0.f };
    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event evt;
        while (window.pollEvent(evt)) {
            ImGui::SFML::ProcessEvent(evt);
            if (evt.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Sample window"); // begin window

        // Background color edit
        if (ImGui::ColorEdit3("Background color", color)) {
            // this code gets called if color value changes, so
            // the background color is upgraded automatically!
            bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
            bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
            bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
        }

        ImGui::End(); // end window
        window.clear(bgColor); // fill background with color
        ImGui::SFML::Render(window);
        //window.clear();

        window.draw(testlines);
        testBlocks.drawContainer(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    delete []CONTRAINT;
    delete []CONTRAINT_RESIZE;
    return 0;
}