cmake_minimum_required(VERSION 3.28)
project(3)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(out
        Entity/desc.cpp
        Entity/desc.h
        Service/fs.cpp
        Service/fs.h
        Table/orderedTable.h
        Table/tableIterator.h
        itValidation.cpp
        Table/node.cpp
        Table/node.h
        Entity/fileDesc.h
        Entity/fileDesc.cpp
        Entity/dirDesc.h
        Entity/dirDesc.cpp
        Entity/spDesc.h
        Entity/spDesc.cpp)


add_executable(test_iter
        Table/tableIterator.h
        Table/orderedTable.h
        itValidation.cpp
)

find_package(Catch2 3 REQUIRED)

add_executable(test_table
        tests/testTable.cpp
        Table/orderedTable.h
        Table/tableIterator.h
)


add_executable(test_class
        tests/testDescs.cpp

        Table/tableIterator.h
        Table/orderedTable.h
        Table/node.cpp
        Table/node.h

        Entity/desc.cpp
        Entity/desc.h

        Entity/fileDesc.h
        Entity/fileDesc.cpp
        Entity/dirDesc.h
        Entity/dirDesc.cpp
        Entity/spDesc.h
        Entity/spDesc.cpp)

add_executable(test_fs
        tests/testFs.cpp

        Table/tableIterator.h
        Table/orderedTable.h
        Table/node.cpp
        Table/node.h

        Service/fs.cpp
        Service/fs.h

        Entity/desc.cpp
        Entity/desc.h

        Entity/fileDesc.h
        Entity/fileDesc.cpp
        Entity/dirDesc.h
        Entity/dirDesc.cpp
        Entity/spDesc.h
        Entity/spDesc.cpp)

add_executable(try
        try.cpp
        Table/tableIterator.h
        Table/orderedTable.h
        Table/node.cpp
        Table/node.h

        Service/fs.cpp
        Service/fs.h

        Entity/desc.cpp
        Entity/desc.h

        Entity/fileDesc.h
        Entity/fileDesc.cpp
        Entity/dirDesc.h
        Entity/dirDesc.cpp
        Entity/spDesc.h
        Entity/spDesc.cpp

        View/view.cpp
        View/view.h
        View/commandDto.h
        Controller/controller.cpp
        Controller/controller.h
)

add_compile_options(-fsanitize=address -fsanitize=undefined)
add_link_options(-fsanitize=address -fsanitize=undefined)

add_executable(benchmark
        benchmark.cpp

        Table/tableIterator.h
        Table/orderedTable.h
        Table/node.cpp
        Table/node.h

        Service/fs.cpp
        Service/fs.h

        Entity/desc.cpp
        Entity/desc.h

        Entity/fileDesc.h
        Entity/fileDesc.cpp
        Entity/dirDesc.h
        Entity/dirDesc.cpp
        Entity/spDesc.h
        Entity/spDesc.cpp)

target_link_libraries(test_table PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_class PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_fs PRIVATE Catch2::Catch2WithMain)