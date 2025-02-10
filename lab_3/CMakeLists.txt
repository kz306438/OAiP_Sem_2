cmake_minimum_required(VERSION 3.10)

project(lab3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    main.cpp
    LinkedList.h
    Sort.h
    Stack.h
)

add_executable(StackProject ${SOURCE_FILES})

target_include_directories(StackProject PRIVATE ${CMAKE_SOURCE_DIR})
