cmake_minimum_required(VERSION 3.21)
project(my_project)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.cxx" "*.cc")
file(GLOB_RECURSE HEADER_FILES "*.h" "*.hpp" "*.hxx")

add_executable(my_project main.cpp crossword.cpp func.cpp crossword.h func.h)