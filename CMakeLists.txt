cmake_minimum_required(VERSION 3.22)
project(AlgoViz)

set(CMAKE_CXX_STANDARD 20)

add_executable(AlgoViz
        main.cpp
        Node.cpp
        Edge.cpp Graph.cpp Graph.h Edge.h Node.h Toolbar.cpp Visualizer.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS graphics REQUIRED)
INCLUDE_DIRECTORIES(C:/SFML/include)
target_link_libraries(AlgoViz sfml-graphics)
