cmake_minimum_required(VERSION 3.10)

project(Lab6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    main.cpp
    Map.hpp
)

add_executable(Lab6 ${SOURCES})
