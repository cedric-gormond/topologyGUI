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
#include <ctime>

#include "block.h"
#include "blockContainer.h"
#include "lineContainer.h"
#include "ctr.hpp"
#include "get_from_ctr.hpp"
#include "file_tools.hpp"
#include "topology.hpp"
#include "create_output_file.hpp"
#include "log.h"
#include "get_time.h"





using namespace std;

//Global
string current_time;

int main() {
    // ---- FILE INPUT ---
    //Please past the path's file
    string file_path = "/Users/cedricgormond/Desktop/topologyGUI/fichier_contrainte_2D.txt"; //FULL PATH

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

    // Display resized blocs in console
    displayBlocs_from_ctr(CONTRAINT_RESIZE, nb_pblocs);

    // ------ OUTPUT FILE ------
    string output_path = input_filename + "_generated.txt";
    ofstream file_output;

    // ------ GUI --------
    sf::RenderWindow window(sf::VideoMode(1200, 1200), "topologyGUI");
    window.setFramerateLimit(30);
    //window.setVerticalSyncEnabled(false);

    //ImGui init
    ImGui::SFML::Init(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    //Theme
    ImGui::StyleColorsLight();

    //
    static bool show_app_log = true;
    static ExampleAppLog my_log;

    //Font
    //io.Fonts->AddFontFromFileTTF(".../misc/fonts/Roboto-Medium.ttf", 16.0f);

    // create container with 5 blocks in it, starting at pos 10/10
    // this container will be drawn using ContainerOfBlocks' void drawContainer(sf::RenderWindow &window)
    //ContainerOfBlocks testBlocks(nb_pblocs, sf::Vector2f(100.0f, 100.0f),CONTRAINT_RESIZE);

    // create  line container, starting
    //ContainerOfLines testlines(nb_pblocs, sf::Vector2f(150.0f, 100.0f), CONTRAINT_RESIZE,100);

    //distance
    int distance = 100;

    //ImGUI Clock
    sf::Clock deltaClock;

    // Infinite loop
    while (window.isOpen()) {
        current_time = currentDateTime(); //get time
        sf::Event evt;
        while (window.pollEvent(evt)) {
            ImGui::SFML::ProcessEvent(evt);
            if (evt.type == sf::Event::Closed) {
                // Resize

                //CONTRAINT_RESIZE = resize_2D_from_bloc1(CONTRAINT, distance, nb_pblocs);
                //ContainerOfBlocks testBlocks(nb_pblocs, sf::Vector2f(100.0f, 100.0f),CONTRAINT_RESIZE);
                //ContainerOfLines testlines(nb_pblocs, sf::Vector2f(150.0f, 100.0f), CONTRAINT_RESIZE,distance);

                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Settings"); // begin window



        //Choose your topology
        ImGui::Text("Choose your topology :");
        static int choice_topology = 0;
        ImGui::RadioButton("2D Mesh", &choice_topology, 0); ImGui::SameLine();
        ImGui::RadioButton("2D Hexa", &choice_topology, 1); ImGui::SameLine();
        ImGui::RadioButton("3D Mesh", &choice_topology, 2);

        // switch topology
        switch(choice_topology) {
            case 0 :
                //Distance
                ImGui::Text("Distance (slices) : ");
                ImGui::InputInt("",&distance);
                break;       // and exits the switch
            case 1 :
                //Distance
                ImGui::Text("Radius (slices) : ");
                ImGui::InputInt("",&distance);

                break;
        }
        ImGui::Separator();

        // Resize dimensions
        //Height
        ImGui::Text("Height of every pblocks (slices) : ");
        //ImGui::InputInt("",&distance);

        //Width
        ImGui::Text("Width of of every pblocks (slices) : ");
        //ImGui::InputInt("",&distance);

        ImGui::Separator();
        if (ImGui::Button("Generate constraint file"))     {
            if (file_output) {
                create_ctr_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                file_output.close();
                my_log.AddLog("%s [info] Generate constraint file \n", &current_time[0]);
                my_log.AddLog( "%s [info] Success \n",&current_time[0]);
                my_log.AddLog( "%s [info] Output file : %s \n", &current_time[0],&output_path[0]);
            } else {
                my_log.AddLog( "%s [error] Cannot write \n", &current_time[0]);
            }
        }

        if (ImGui::Button("Generate constraint file (simplified)"))     {
            my_log.AddLog("%s [info] Generate simplified constraint file \n", &current_time[0]);
        }

        //LOG DISPLAU IN CURRENT WINDOW
        ImGui::Separator();
        ImGui::Text("Log :");
        my_log.DrawInCurrentWindow("Log");
        ImGui::End(); // end window

        window.clear();
        ImGui::SFML::Render(window);

        //window.draw(testBlocks);
        //window.draw(testlines);
        //testBlocks.drawContainer(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    delete []CONTRAINT;
    delete []CONTRAINT_RESIZE;
    return 0;
}