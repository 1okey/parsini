cmake_minimum_required(VERSION 3.0)

project(parsini_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(include)

add_executable(
    ${PROJECT_NAME}
    test.cpp
)

target_link_libraries(parsini_test parsini)

