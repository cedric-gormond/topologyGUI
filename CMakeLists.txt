cmake_minimum_required(VERSION 3.12)
project(topologyGUI)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES
        src/main.cpp
        src/block.cpp
        src/block.h
        src/blockContainer.cpp
        src/blockContainer.h
        src/line.cpp
        src/line.h
        src/lineContainer.cpp
        src/lineContainer.h
        src/file_tools.cpp
        src/file_tools.hpp
        src/create_output_file.cpp
        src/create_output_file.hpp
        src/ctr.cpp
        src/ctr.hpp
        src/get_from_ctr.cpp
        src/get_from_ctr.hpp
        src/get_from_txt.cpp
        src/get_from_txt.hpp
        src/topology.cpp
        src/topology.hpp
        src/gen_tools.cpp
        src/gen_tools.h
        imgui/imconfig.h
        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui-sfml/imconfig-SFML.h
        imgui-sfml/imgui-SFML.cpp
        imgui-sfml/imgui-SFML.h
        src/log.h src/get_time.cpp
        src/get_time.h
        src/surface.cpp src/surface.h)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++") #a voir

add_executable(topologyGUI ${SOURCE_FILES})

# Boost
set(boost_min_ver 1.50.0)
set(boost_libs system filesystem)
find_package(Boost ${boost_min_ver})

if(Boost_FOUND)
    find_package(Boost ${boost_min_ver} COMPONENTS ${boost_libs})
endif()

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
##

# ImGui
include_directories(imgui)

# ImGui/SFML
include_directories(imgui-sfml)

# OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# SFML
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(topologyGUI ${SFML_LIBRARIES})
endif()



# Imgui
# ImGui does not provide native support for CMakeLists, workaround for now to have
# users specify IMGUI_ROOT. See: https://github.com/ocornut/imgui/pull/255
if(NOT IMGUI_ROOT)
    set(IMGUI_ROOT "" CACHE PATH "imgui top-level directory")
    message("---> ImGui directory not found. Set IMGUI_ROOT to imgui's top-level path (containing \"imgui.cpp\" and \"imgui.h\" files).\n")
endif()

# Do a pseudo find files for ImGui once IMGUI_ROOT is set
if(IMGUI_ROOT)
    set(IMGUI_SERACH_PATH
            ${IMGUI_ROOT}
            $ENV{IMGUI_ROOT}
            )
    find_path(IMGUI_INCLUDE_DIR imgui.cpp
            PATHS ${IMGUI_SERACH_PATH})
    if(NOT IMGUI_INCLUDE_DIR)
        message(FATAL_ERROR "---> IMGUI imgui.cpp not found. Set IMGUI_ROOT to imgui's top-level path (containing \"imgui.cpp\" and \"imgui.h\" files).\n")
    else()
        file(GLOB IMGUI_FILES
                "${IMGUI_INCLUDE_DIR}/imgui_draw.cpp"
                "${IMGUI_INCLUDE_DIR}/imgui_demo.cpp"
                "${IMGUI_INCLUDE_DIR}/imgui.cpp" )
        message("Found imgui.cpp in ${IMGUI_INCLUDE_DIR}")
        # Rename that pesky imconfig.h file for the user.
        install(FILES ${IMGUI_INCLUDE_DIR}/imconfig.h DESTINATION include RENAME imconfig-sample.h)
    endif()
endif()

# Glob up both source and headers as sources for VS users.
file(GLOB IMGUI_SFML_FILES "${PROJECT_SOURCE_DIR}/*.cpp" "${PROJECT_SOURCE_DIR}/*.h")

# Set these for users to use
set(IMGUI_SFML_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}
        ${IMGUI_INCLUDE_DIR}
        ${SFML_INCLUDE_DIR} CACHE INTERNAL "")

set(IMGUI_SOURCES
        ${IMGUI_FILES} CACHE INTERNAL "")

set(IMGUI_SFML_SOURCES
        ${IMGUI_SFML_FILES} CACHE INTERNAL "")

set(IMGUI_SFML_DEPENDENCIES
        ${SFML_DEPENDENCIES}
        ${SFML_LIBRARIES} CACHE INTERNAL "")

if(IMGUI_SFML_BUILD_EXAMPLES)
    # Build examples
    add_subdirectory(examples)
endif(IMGUI_SFML_BUILD_EXAMPLES)








