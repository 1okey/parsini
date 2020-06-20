cmake_minimum_required(VERSION 3.0)

project(parsini VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(include)

add_subdirectory(test)

add_library (${PROJECT_NAME} src/inidoc.cpp)