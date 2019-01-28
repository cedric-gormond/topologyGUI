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

        cout << endl << " * The constraint file must be named : fichier_contrainte_2D.txt \n \n";
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
    //displayBlocs_from_ctr(CONTRAINT_RESIZE, nb_pblocs);

    // ------ OUTPUT FILE ------
    string output_path = input_filename + "_generated.txt";
    ofstream file_output;

    // ------ SFML --------
    sf::RenderWindow window(sf::VideoMode(1200, 1200), "topologyGUI", sf::Style::Close);
    window.setFramerateLimit(30);

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
    static bool* p_open = new bool;
    *p_open = true;

    //Font
    //io.Fonts->AddFontDefault();
    ImFont* font = io.Fonts->AddFontFromFileTTF("misc/fonts/Roboto-Medium.ttf", 16.0f);
    IM_ASSERT(font != NULL);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("misc/fonts/Roboto-Medium.ttf", 16.0f);


    // create container with 5 blocks in it, starting at pos 10/10
    // this container will be drawn using ContainerOfBlocks' void drawContainer(sf::RenderWindow &window)
    //ContainerOfBlocks testBlocks(nb_pblocs, sf::Vector2f(100.0f, 100.0f),CONTRAINT_RESIZE);

    // create  line container, starting
    //ContainerOfLines testlines(nb_pblocs, sf::Vector2f(150.0f, 100.0f), CONTRAINT_RESIZE,100);

    //user's variables
    int distance = 100;
    int radius = 50;

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

        //Bin IMGUI
        ImGui::Begin("Settings",p_open,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        ImGui::Spacing();
        if (ImGui::CollapsingHeader("Help"))
        {

        }

        ImGui::Text("Current file : %s", &file_path[0]);

        //Choose your topology
        static int choice_topology = 0;
        ImGui::Spacing();
        if (ImGui::CollapsingHeader("Topology"))
        {
            ImGui::Text("Choose your topology :");

            ImGui::RadioButton("2D Mesh", &choice_topology, 1); ImGui::SameLine();
            ImGui::RadioButton("2D Hexa", &choice_topology, 2); ImGui::SameLine();
            ImGui::RadioButton("3D Mesh", &choice_topology, 3);

            switch(choice_topology) {
                case 1 :
                    ImGui::Text("Distance (slices) : ");
                    ImGui::InputInt("",&distance);
                    break;
                case 2 :
                    //Radius
                    ImGui::Text("Radius (slices) : ");
                    ImGui::InputInt("",&radius);
                    break;
            }
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
            // switch topology
            switch(choice_topology) {
                case 1 :
                    ImGui::Text("Distance (slices) : ");
                    ImGui::InputInt("",&distance);

                    CONTRAINT_RESIZE = resize_2D_from_bloc1(CONTRAINT,distance, nb_pblocs);

                    //Writing
                    file_output.open(output_path);
                    create_ctr_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    file_output.close();

                    //log
                    my_log.AddLog("%s [info] Generate constraint file (2D MESH)\n", &current_time[0]);
                    my_log.AddLog( "%s [info] Success \n",&current_time[0]);
                    my_log.AddLog( "%s [info] Output file : %s \n", &current_time[0],&output_path[0]);
                    break;
                case 2 :
                    //Radius
                    ImGui::Text("Radius (slices) : ");
                    ImGui::InputInt("",&radius);

                    CONTRAINT_RESIZE = set_hexa(CONTRAINT,radius, nb_pblocs);

                    //Writing
                    file_output.open(output_path);
                    create_ctr_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    file_output.close();

                    //log
                    my_log.AddLog("%s [info] Generate constraint file (2D HEXA)\n", &current_time[0]);
                    my_log.AddLog( "%s [info] Success \n",&current_time[0]);
                    my_log.AddLog( "%s [info] Output file : %s \n", &current_time[0],&output_path[0]);
                    break;

                default:
                    my_log.AddLog("%s [error] Please choose a topology\n", &current_time[0]);
                    break;
            }

            /*
            if (file_output) {

                file_output.open(output_path);
                create_ctr_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                file_output.close();

                my_log.AddLog("%s [info] Generate constraint file \n", &current_time[0]);
                my_log.AddLog( "%s [info] Success \n",&current_time[0]);
                my_log.AddLog( "%s [info] Output file : %s \n", &current_time[0],&output_path[0]);
            } else {
                my_log.AddLog( "%s [error] Cannot write \n", &current_time[0]);
            }
             */
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