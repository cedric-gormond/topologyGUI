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
#include "graphics_utilities.h"

using namespace std;

//Global
string current_time;

int main() {

    // ---- FILE INPUT ---
    //Please past the path's file ;"/Users/cedricgormond/Desktop/topologyGUI/fichier_contrainte_2D.txt"
    string file_path = "fichier_contrainte_2D.txt"; //FULL PATH

    ifstream file(file_path.c_str(), ios::in);
    string input_filename = getFilename(file_path); // get the input file name WITHOUT extension

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
    auto *CONTRAINT = new constraint[nb_pblocs];
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
    auto *CONTRAINT_RESIZE = new constraint[nb_pblocs];
    initConstraint(CONTRAINT_RESIZE);
    CONTRAINT_RESIZE = CONTRAINT; //copy

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
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

    // ImGUI - variables
    static ExampleAppLog my_log;
    static bool* p_open = new bool;
    *p_open = true;

    //Font
    //io.Fonts->AddFontDefault();
    //ImFont* font = io.Fonts->AddFontFromFileTTF("misc/fonts/Roboto-Medium.ttf", 16.0, nullptr);
    //IM_ASSERT(font != NULL);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("misc/fonts/Roboto-Medium.ttf", 16.0f);

    //ImGUI Clock
    sf::Clock deltaClock;

    /*
     *  SFML Object
     */
    // create container with nb_pblocs blocks in it, starting at pos 100/100
    // this container will be drawn using ContainerOfBlocks' void drawContainer(sf::RenderWindow &window)
    // ContainerOfBlocks Blocks(nb_pblocs, sf::Vector2f(100, 100),CONTRAINT_RESIZE);

    // create  line container, starting at pos 100/100
    // this container will be drawn using ContainerOfLines' void drawContainer(sf::RenderWindow &window)
    // ContainerOfLines lines(nb_pblocs, sf::Vector2f(150.0f, 100.0f), CONTRAINT_RESIZE,100);

    /*
     *  User's variables
     */
    static int distance2D    =  100;
    static int distance3D    =  100;
    static int radius        =  50;
    *p_open                  =  true;

    /*
     * ---------------------------------------------------------------
     *                      MAIN LOOP
     * ---------------------------------------------------------------
     */
    // Infinite loop
    while (window.isOpen()) {
        current_time = currentDateTime(); //get time
        sf::Event evt;
        while (window.pollEvent(evt)) {
            ImGui::SFML::ProcessEvent(evt);

            //ContainerOfLines lines(nb_pblocs, sf::Vector2f(150.0f, 100.0f), CONTRAINT_RESIZE,distance);

            if (evt.type == sf::Event::Closed) {
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
            ImGui::BulletText("TopologyGUI can only generate constraint files with their respective dimensions. \n");
        }
        ImGui::Separator();


        /*
         *  Open file
         */
        if (ImGui::CollapsingHeader("Open file"))
        {
            ImGui::BulletText("WIP (à faire un collapsed ou en menu)");

            /*
            if (file) {
                my_log.AddLog("%s [info] File found : \n", &current_time[0]);
                my_log.AddLog("%s",&file_path[0]);
            }
            else {
                my_log.AddLog("%s [error] Cannot find  :",&current_time[0]);
                my_log.AddLog("%s",&file_path[0]);

                my_log.AddLog("%s [suggest] The constraint file must be named : fichier_contrainte_2D.txt \n",&current_time[0]);
                my_log.AddLog("%s [info] Exit : \n", &current_time[0]);
            }
             */
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
         *  Create constraint file from scratch
         */
        static bool disabled = true;
        static int gens[2] = { 3, 3 }; // X0 Y0 X1 Y1
        static int coord[4] = { 10, 20 , 20, 30}; // X0 Y0 X1 Y1
        if (ImGui::CollapsingHeader("Create constraint file from scratch"))
        {
            ImGui::BeginGroup();
            ImGui::Checkbox("Disable", &disabled);
            if (disabled)   ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

            ImGui::Text("DimX :                          "); ImGui::SameLine();
            ImGui::Text("DimY :                          ");
            ImGui::InputInt2("##", gens);
            //ImGui::Text("%s Blocs", nb_blocs[0]);

            ImGui::Text("X0 :           "); ImGui::SameLine();
            ImGui::Text("Y0 :            "); ImGui::SameLine();
            ImGui::Text("X1 :           "); ImGui::SameLine();
            ImGui::Text("Y1 : ");
            ImGui::InputInt4("##", coord);

            if (disabled)   ImGui::PopStyleVar();
            ImGui::EndGroup();
        }
        ImGui::Separator();

        /*
         *  Topology
         */
        static int choice_topology; //Choose your topology
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
                    ImGui::InputInt("",&distance2D);
                    break;
                case 2 :
                    //Radius
                    ImGui::Text("Radius (in slices) between the center (block1) and each other block : ");
                    ImGui::InputInt("",&radius);
                    break;
                case 3 :
                    //Radius
                    ImGui::Text("Distance (in slices) between each block : ");
                    ImGui::InputInt("",&distance3D);
                    ImGui::Text("ATTENTION ! TopologyUI can only handle two Z generations (from 0 to 1)");
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
        static int vec2i[2] = { 10, 10 };
        static int default2i[2] = { CONTRAINT[0].width, CONTRAINT[0].heigth};

        if (ImGui::CollapsingHeader("Dimensions"))
        {
            ImGui::RadioButton("Use default size", &choice_resize, 1);ImGui::SameLine();
            ImGui::RadioButton("Resize every pblock", &choice_resize, 2);

            switch (choice_resize) {
                case 1 :
                    ImGui::Text("(%d , %d)", default2i[0],default2i[1] );
                    break;

                case 2:
                    //Width
                    ImGui::Text("Width :                         "); //nasty spacing (^.^)
                    ImGui::SameLine();
                    ImGui::Text("Heigth : ");
                    ImGui::InputInt2("##", vec2i);
                    break;
            }
        }
        ImGui::Separator();
        ImGui::Spacing();

        /*
         *  Generate constraint file
         */
        ImGui::Text("Save constraint file as ");
        const char* items[] = { ".txt", ".xdc"};
        static int choice_type = 0;
        ImGui::Combo("", &choice_type, items, IM_ARRAYSIZE(items));
        auto *CONTRAINT_CREATED = new constraint[gens[0]*gens[1]];

        if (ImGui::Button("Generate constraint file"))     {
            std::string output_path_temp;
            // switch topology
            switch(choice_topology) {
                case 1 :
                    //check resize dimensions
                    if(choice_resize == 2){
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, vec2i);
                        my_log.AddLog("%s [info] [mesh2D] Resize every blocs to (%d,%d) \n", &current_time[0], vec2i[0], vec2i[1]);
                    } else {
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, default2i);
                    }

                    //apply topology

                    if (disabled){
                        CONTRAINT_RESIZE = set_2D_from_bloc1(CONTRAINT_RESIZE,distance2D, nb_pblocs);
                    }
                    else{
                        my_log.AddLog("%s [info] [mesh2D] Input file ignored \n", &current_time[0]);
                        CONTRAINT_CREATED = CreateConstraint(gens,coord);
                        CONTRAINT_CREATED = set_2D_from_bloc1(CONTRAINT_CREATED,distance2D, nb_pblocs);
                    }

                    //Writing
                    if (choice_type == 0) output_path_temp = output_path + "_generated.txt";
                    if (choice_type == 1) output_path_temp = output_path + "_generated.xdc";
                    file_output.open(output_path_temp);
                    if (disabled)
                        create_ctr_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    else
                        create_ctr_file(file_output, file_path, CONTRAINT_CREATED, nb_pblocs);
                    file_output.close();

                    //log
                    my_log.AddLog("%s [info] [mesh2D] Generate constraint file \n", &current_time[0]);
                    my_log.AddLog("%s [info] [mesh2D] Success \n",&current_time[0]);
                    my_log.AddLog("%s [info] [mesh2D] Output file : %s \n", &current_time[0],&output_path_temp[0]);
                    break;
                case 2 :
                    //check resize dimensions
                    if(choice_resize == 2){
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, vec2i);
                        my_log.AddLog("%s [info] [hexa] Resize every blocs to (%d,%d) \n", &current_time[0], vec2i[0], vec2i[1]);
                    }else {
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, default2i);
                    }

                    //apply topology
                    CONTRAINT_RESIZE = set_hexa(CONTRAINT_RESIZE,radius, nb_pblocs);

                    //Writing
                    if (choice_type == 0) output_path_temp = output_path + "_hexa_generated.txt";
                    if (choice_type == 1) output_path_temp = output_path + "_hexa_generated.xdc";
                    file_output.open(output_path_temp);
                    create_ctr_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    file_output.close();

                    //log
                    my_log.AddLog("%s [info] [hexa] Generate constraint file \n", &current_time[0]);
                    my_log.AddLog("%s [info] [hexa] Success \n",&current_time[0]);
                    my_log.AddLog("%s [info] [hexa] Output file : %s \n", &current_time[0],&output_path_temp[0]);
                    break;
                case 3:
                    //check resize dimensions
                    if(choice_resize == 3){
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, vec2i);
                        my_log.AddLog("%s [info] [mesh3D] Resize every blocs to (%d,%d) \n", &current_time[0], vec2i[0], vec2i[1]);
                    } else {
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, default2i);
                    }

                    //apply topology
                    CONTRAINT_RESIZE = set_3D(CONTRAINT_RESIZE,distance3D, nb_pblocs);

                    //Writing
                    if (choice_type == 0) output_path_temp = output_path + "_3D_generated.txt";
                    if (choice_type == 1) output_path_temp = output_path + "_3D_generated.xdc";
                    file_output.open(output_path_temp);
                    create_ctr_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    file_output.close();

                    //log
                    my_log.AddLog("%s [info] [mesh3D] Generate constraint file \n", &current_time[0]);
                    my_log.AddLog("%s [info] [mesh3D] Success \n",&current_time[0]);
                    my_log.AddLog("%s [info] [mesh3D] Output file : %s \n", &current_time[0],&output_path_temp[0]);
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
                    //check resize dimensions
                    if(choice_resize == 2){
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, vec2i);
                        my_log.AddLog("%s [info] [mesh2D] Resize every blocs to (%d,%d) \n", &current_time[0], vec2i[0], vec2i[1]);
                    }else {
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, default2i);
                    }

                    //apply topology
                    CONTRAINT_RESIZE = set_2D_from_bloc1(CONTRAINT_RESIZE,distance2D, nb_pblocs);

                    //Writing
                    file_output.open(output_path_temp);
                    create_txt_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    file_output.close();

                    //log
                    output_path_temp = output_path + "_simplified_generated.txt";
                    my_log.AddLog("%s [info] [mesh2D] Generate simplified constraint file \n", &current_time[0]);
                    my_log.AddLog("%s [info] [mesh2D] Success \n",&current_time[0]);
                    my_log.AddLog("%s [info] [mesh2D] Output file : %s \n", &current_time[0],&output_path_temp[0]);
                    break;

                case 2:
                    //check resize dimensions
                    if(choice_resize == 2){
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, vec2i);
                        my_log.AddLog("%s [info] [hexa] Resize every blocs to (%d,%d) \n", &current_time[0], vec2i[0], vec2i[1]);
                    }else {
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, default2i);
                    }
                    //apply topology
                    CONTRAINT_RESIZE = set_hexa(CONTRAINT_RESIZE,radius, nb_pblocs);

                    //Writing
                    output_path_temp = output_path + "_simplified_generated.txt";
                    file_output.open(output_path_temp);
                    create_txt_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    file_output.close();

                    //log
                    my_log.AddLog("%s [info] [HEXA] Generate simplified constraint file \n", &current_time[0]);
                    my_log.AddLog("%s [info] [HEXA] Success \n",&current_time[0]);
                    my_log.AddLog("%s [info] [HEXA] Output file : %s \n", &current_time[0],&output_path_temp[0]);
                    break;
                case 3:
                    //check resize dimensions
                    if(choice_resize == 3){
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, vec2i);
                        my_log.AddLog("%s [info] [mesh3D] Resize every blocs to (%d,%d) \n", &current_time[0], vec2i[0], vec2i[1]);
                    } else {
                        resize_dimensions(CONTRAINT_RESIZE, nb_pblocs, default2i);
                    }
                    //apply topology
                    CONTRAINT_RESIZE = set_3D(CONTRAINT_RESIZE,distance3D, nb_pblocs);

                    //Writing
                    output_path_temp = output_path + "_simplified_generated.txt";
                    file_output.open(output_path_temp);
                    create_txt_file(file_output, file_path, CONTRAINT_RESIZE, nb_pblocs);
                    file_output.close();

                    //log
                    my_log.AddLog("%s [info] [mesh3D] Generate simplified constraint file \n", &current_time[0]);
                    my_log.AddLog("%s [info] [mesh3D] Success \n",&current_time[0]);
                    my_log.AddLog("%s [info] [mesh3D] Output file : %s \n", &current_time[0],&output_path_temp[0]);
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
        //Blocks.drawContainer(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    //delete []CONTRAINT;
    //delete []CONTRAINT_RESIZE;
    return 0;
}