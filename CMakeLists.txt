cmake_minimum_required(VERSION 3.22)
project(AlgoViz)

set(CMAKE_CXX_STANDARD 20)

add_executable(AlgoViz main.cpp Node.cpp Edge.cpp Graph.cpp Toolbar.cpp Toolbar.h Visualizer.cpp Visualizer.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/Users/Maysam/Documents/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS graphics system window network REQUIRED)
INCLUDE_DIRECTORIES(C:/Users/Maysam/Documents/SFML/include)
target_link_libraries(AlgoViz sfml-graphics)
