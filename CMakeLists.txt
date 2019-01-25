cmake_minimum_required(VERSION 3.12)
project(topologyGUI)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/block.cpp src/block.h src/blockContainer.cpp src/blockContainer.h src/line.cpp src/line.h src/lineContainer.cpp src/lineContainer.h src/file_tools.cpp src/file_tools.hpp src/create_output_file.cpp src/create_output_file.hpp src/ctr.cpp src/ctr.hpp src/get_from_ctr.cpp src/get_from_ctr.hpp src/get_from_txt.cpp src/get_from_txt.hpp src/topology.cpp src/topology.hpp)

add_executable(topologyGUI ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(topologyGUI ${SFML_LIBRARIES})
endif()









