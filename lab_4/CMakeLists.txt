cmake_minimum_required(VERSION 3.10)

project(Lab4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    Deque.h
    List.h
    Sort.h
    TestUtils.h
    main.cpp
)

add_executable(Lab4 ${SOURCES})
