//
// Created by pavel on 14.01.2026.
//

#include <chrono>
#include <iostream>

#include "Service/fs.h"


int main() {

    constexpr size_t total_count = 100;

    Fs fs;
    fs.generate(50000);
    //fs.upload("file5");

    //fs.download("file4");

    size_t total = 0;
    int count = 0;


    std::vector<EntityDto> res;
    std::cout << "Testing single thread version" << std::endl;
    for (; count < total_count; ++count) {
        auto start = std::chrono::steady_clock::now();
        res = fs.find("mv3Jf0cOYx");
        auto end = std::chrono::steady_clock::now();
        total += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
    std::cout << "Found: " << res.size() << " items" << std::endl;
    std::cout << "Time for one search in single mode: " << total / total_count  << " microseconds" << std::endl;





    std::cout << "Testing multithread version" << std::endl;
    total = 0;
    count = 0;
    for (; count < total_count; ++count) {
        auto start = std::chrono::steady_clock::now();
        res = fs.find_threads("mv3Jf0cOYx");
        auto end = std::chrono::steady_clock::now();
        total += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
    std::cout << "Found: " << res.size() << " items" << std::endl;
    std::cout << "Time for one search in multithread mode: " << total / total_count << " microseconds" << std::endl;




    return 0;
}