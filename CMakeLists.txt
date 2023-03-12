cmake_minimum_required(VERSION 3.24)
project(RailwayManagement)

set(CMAKE_CXX_STANDARD 17)

add_executable(RailwayManagement src/main.cpp src/station.h src/menu.h src/menu.cpp src/station.cpp src/edge.h src/edge.cpp src/vertex.h src/vertex.cpp src/graph.cpp)
