cmake_minimum_required(VERSION 3.28)
project(2)

set(CMAKE_CXX_STANDARD 23)

add_executable(out main.cpp
        gate.cpp
        gate.h
        dialog.cpp
        dialog.h
        pin.h
        pin.cpp)



# Tests
find_package(Catch2 3 REQUIRED)

add_executable(test
        tests/testing.cpp
        gate.h
        gate.cpp
        pin.h
        pin.cpp
)

#target_include_directories(test PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(test PRIVATE Catch2::Catch2WithMain)

