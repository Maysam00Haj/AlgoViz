cmake_minimum_required(VERSION 3.22)
project(AlgoViz)

set(CMAKE_CXX_STANDARD 14)

add_executable(AlgoViz main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/Users/Maysam/Documents/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS graphics REQUIRED)
INCLUDE_DIRECTORIES(C:/Users/Maysam/Documents/SFML/include)
target_link_libraries(AlgoViz sfml-graphics)
