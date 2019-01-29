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

#include "get_time.h"
#include "log.h"

using namespace std;

//Global
string current_time;

int main() {

    // ---- FILE INPUT ---
    //Please past the path's file
    string file_path = "fichier_contrainte_2D.txt"; //FULL PATH

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
    //constraint *CONTRAINT_RESIZE = set_hexa(CONTRAINT, 50, nb_pblocs);
    constraint *CONTRAINT_RESIZE = set_2D_from_bloc1(CONTRAINT, 100, nb_pblocs);

    // ------ OUTPUT FILE ------
    string output_path = input_filename;
    ofstream file_output;

    /*
     * ---------------------------------------------------------------
     *                             SFML
     * ---------------------------------------------------------------
     */

    /*
     *  Init SFML
     */
    //Window settings
    sf::RenderWindow window(sf::VideoMode(700, 700), "topologyGUI", sf::Style::Close);
    window.setFramerateLimit(30);

    /*
     *  Init ImGUI
     */
    ImGui::SFML::Init(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    //Theme
    ImGui::StyleColorsLight();

    // ImGUI - variables
    static ExampleAppLog my_log;
    static bool show_app_log    = true;
    static bool* p_open = new bool;
    *p_open = true;

    //Font
    //io.Fonts->AddFontDefault();
    ImFont* font = io.Fonts->AddFontFromFileTTF("misc/fonts/Roboto-Medium.ttf", 16.0, nullptr);
    IM_ASSERT(font != NULL);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("misc/fonts/Roboto-Medium.ttf", 16.0f);

    //ImGUI Clock
    sf::Clock deltaClock;

    /*
     *  SFML Object
     */
    // create container with nb_pblocs blocks in it, starting at pos 100/100
    // this container will be drawn using ContainerOfBlocks' void drawContainer(sf::RenderWindow &window)
    //ContainerOfBlocks Blocks(nb_pblocs, sf::Vector2f(100.0f, 100.0f),CONTRAINT_RESIZE);

    // create  line container, starting at pos 100/100
    // this container will be drawn using ContainerOfLines' void drawContainer(sf::RenderWindow &window)
    //ContainerOfLines lines(nb_pblocs, sf::Vector2f(150.0f, 100.0f), CONTRAINT_RESIZE,100);

    /*
     *  User's variables
     */
    //
    int distance    =   100;
    int radius      =   50;
    int width_temp  =   0;
    int heigth_temp =   0;

    /*
     *  MAIN LOOP
     */
    // Infinite loop
    while (window.isOpen()) {
        current_time = currentDateTime(); //get time
        sf::Event evt;
        while (window.pollEvent(evt)) {
            ImGui::SFML::ProcessEvent(evt);
            if (evt.type == sf::Event::Closed) {
                //CONTRAINT_RESIZE = resize_2D_from_bloc1(CONTRAINT, distance, nb_pblocs);
                //ContainerOfBlocks Blocks(nb_pblocs, sf::Vector2f(100.0f, 100.0f),CONTRAINT);
                //ContainerOfLines lines(nb_pblocs, sf::Vector2f(150.0f, 100.0f), CONTRAINT_RESIZE,distance);
                window.close();
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        /*
         * ---------------------------------------------------------------
         *                      ImGUI - User Interface
         * ---------------------------------------------------------------
         */

        /*
         *  Begin main window
         */
        ImGui::Begin("Settings",p_open,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Current file : %s", &file_path[0]);

        /*
         *  Help section
         */
        ImGui::Spacing();
        if (ImGui::CollapsingHeader("Help")) {
            ImGui::BulletText("TopologyGUI doesn't recognize .xdc files ");
        }
        ImGui::Separator();


        /*
         *  Open file
         */
        if (ImGui::CollapsingHeader("Open file"))
        {
            ImGui::BulletText("WIP (à faire un collapsed ou en menu");
        }

        /*
         *  Generate simplified constraint file without any modification
         */
        ImGui::Text("Generate simplified constraint file without any modification");
        if (ImGui::Button("Generate simplified constraint file "))     {
            std::string output_path_temp = output_path + "_original_simplified.txt";

            //Writing
            file_output.open(output_path_temp);
            create_txt_file(file_output, file_path, CONTRAINT, nb_pblocs);
            file_output.close();

            //log
            my_log.AddLog("%s [info] [default] Generate simplified constraint file \n", &current_time[0]);
            my_log.AddLog("%s [info] [default] Success \n",&current_time[0]);
            my_log.AddLog("%s [info] [default] Output file : %s \n", &current_time[0],&output_path_temp[0]);
        }
        ImGui::SameLine();
        ShowHelpMarker("Generate a simplified constraint file which is NOT compatible with Xilinx Vivado\n");
        ImGui::Separator();
        ImGui::Spacing();

        /*
         *  Topology
         */
        static int choice_topology = 0; //Choose your topology
        ImGui::Spacing();
        if (ImGui::CollapsingHeader("Topology"))
        {
            ImGui::Text("Choose your topology :");

            ImGui::RadioButton("2D Mesh", &choice_topology, 1); ImGui::SameLine();
            ImGui::RadioButton("2D Hexa", &choice_topology, 2); ImGui::SameLine();
            ImGui::RadioButton("3D Mesh", &choice_topology, 3);

            switch(choice_topology) {
                case 1 :
                    ImGui::Text("Distance (in slices) between each block : ");
                    ImGui::InputInt("",&distance);
                    break;
                case 2 :
                    //Radius
                    ImGui::Text("Radius (in slices) between the center (block1) and each other block : ");
                    ImGui::InputInt("",&radius);
                    break;
            }
        }
        ImGui::Separator();
        ImGui::Spacing();

        /*
         *  Dimensions
         */
        // Resize dimensions
        static int choice_resize = 0;
        if (ImGui::CollapsingHeader("Dimensions"))
        {
            ImGui::RadioButton("Use default size", &choice_resize, 1);ImGui::SameLine();
            //ImGui::Text("(%d , %d )",CONTRAINT_RESIZE[0].heigth,CONTRAINT_RESIZE[0].width);

            ImGui::RadioButton("Resize every pblock", &choice_resize, 2);

            switch (choice_resize) {
                case 1 :
                    ImGui::Text("(%d , %d)",CONTRAINT_RESIZE[0].heigth,CONTRAINT_RESIZE[0].width);
                    break;

                case 2:
                    //Height
                    //std::string temp1, temp2;
                    ImGui::Text("Height of every pblocks (slices) : ");
                    ImGui::InputInt("##",&width_temp);
                    //heigth_temp = stoi(temp1);

                    //Width
                    ImGui::Text("Width of of every pblocks (slices) : ");
                    ImGui::InputInt("##",&heigth_temp);
                    break;
            }
        }
        ImGui::Separator();
        ImGui::Spacing();

        /*
         *  Generate constraint file
         */
        if (ImGui::Button("Generate constraint file"))     {
            std::string output_path_temp;
            // switch topology
            switch(choice_topology) {
                case 1 :
                    CONTRAINT_RESIZE = set_2D_from_bloc1(CONTRAINT,distance, nb_pblocs);

                    //Writing
                    output_path_temp = output_path + "_generated.txt";
                    file_output.open(output_path_temp);
                    create_ctr_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    file_output.close();

                    //log
                    my_log.AddLog("%s [info] [mesh2D] Generate constraint file \n", &current_time[0]);
                    my_log.AddLog("%s [info] [mesh2D] Success \n",&current_time[0]);
                    my_log.AddLog("%s [info] [mesh2D] Output file : %s \n", &current_time[0],&output_path_temp[0]);
                    break;
                case 2 :
                    CONTRAINT_RESIZE = set_hexa(CONTRAINT,radius, nb_pblocs);

                    //Writing
                    output_path_temp = output_path + "_hexa_generated.txt";
                    file_output.open(output_path_temp);
                    create_ctr_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    file_output.close();

                    //log
                    my_log.AddLog("%s [info] [hexa] Generate constraint file \n", &current_time[0]);
                    my_log.AddLog("%s [info] [hexa] Success \n",&current_time[0]);
                    my_log.AddLog("%s [info] [hexa] Output file : %s \n", &current_time[0],&output_path_temp[0]);
                    break;

                default:
                    my_log.AddLog("%s [error] Please choose a topology\n", &current_time[0]);
                    break;
            }
        }

        /*
         *  Generate constraint file (simplified)
         */
        if (ImGui::Button("Generate constraint file (simplified)"))     {
            std::string output_path_temp;
            switch(choice_topology) {

                case 1:
                    CONTRAINT_RESIZE = set_2D_from_bloc1(CONTRAINT,distance, nb_pblocs);

                    //Writing
                    file_output.open(output_path_temp);
                    create_txt_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    file_output.close();

                    //log
                    my_log.AddLog("%s [info] [mesh2D] Generate simplified constraint file \n", &current_time[0]);
                    my_log.AddLog("%s [info] [mesh2D] Success \n",&current_time[0]);
                    my_log.AddLog("%s [info] [mesh2D] Output file : %s \n", &current_time[0],&output_path_temp[0]);
                    break;

                case 2:
                    CONTRAINT_RESIZE = set_hexa(CONTRAINT,radius, nb_pblocs);

                    //Writing
                    file_output.open(output_path_temp);
                    create_txt_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    file_output.close();

                    //log
                    my_log.AddLog("%s [info] [HEXA] Generate simplified constraint file \n", &current_time[0]);
                    my_log.AddLog("%s [info] [HEXA] Success \n",&current_time[0]);
                    my_log.AddLog("%s [info] [HEXA] Output file : %s \n", &current_time[0],&output_path_temp[0]);
                    break;

                default:
                    my_log.AddLog("%s [error] Please choose a topology\n", &current_time[0]);
                    break;
            }
        }
        ImGui::SameLine();
        ShowHelpMarker("Generate a simplified constraint file which is NOT compatible with Xilinx Vivado\n");
        ImGui::Spacing();
        ImGui::Separator();

        /*
         *  LOG
         */
        ImGui::Text("Log :"); ImGui::SameLine();
        my_log.DrawInCurrentWindow("Log");

        ImGui::End(); // end window

        /*
         *  RENDER
         */
        window.clear();

        ImGui::SFML::Render(window);


        //window.draw(Blocks);
        //window.draw(lines);
        //testBlocks.drawContainer(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    delete []CONTRAINT;
    delete []CONTRAINT_RESIZE;
    return 0;
}