cmake_minimum_required(VERSION 3.22)
project(AlgoViz)

set(CMAKE_CXX_STANDARD 20)

add_executable(AlgoViz src/main.cpp src/Node.cpp src/Edge.cpp src/Graph.cpp src/Toolbar.cpp src/Visualizer.cpp src/utils.cpp src/MessagesBox.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)

find_package(SFML COMPONENTS graphics system window network REQUIRED)
INCLUDE_DIRECTORIES(C:/SFML/include)
target_link_libraries(AlgoViz sfml-graphics)
