cmake_minimum_required(VERSION 3.12)
project(topologyGUI)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp block.cpp block.h blockContainer.cpp blockContainer.h line.cpp line.h lineContainer.cpp lineContainer.h file_tools.cpp file_tools.hpp create_output_file.cpp create_output_file.hpp ctr.cpp ctr.hpp get_from_ctr.cpp get_from_ctr.hpp get_from_txt.cpp get_from_txt.hpp topology.cpp topology.hpp)

add_executable(topologyGUI ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(topologyGUI ${SFML_LIBRARIES})
endif()









