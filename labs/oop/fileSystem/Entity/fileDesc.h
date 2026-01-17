//
// Created by pavel on 11.12.2025.
//

#ifndef INC_3_FILEDESC_H
#define INC_3_FILEDESC_H

#include <chrono>

#include "desc.h"

/**
 * descriptor of file with times
 */
class FileDesc: public Desc {
    std::time_t creation_time;
    std::time_t modification_time;

public:
    FileDesc() : FileDesc(std::bitset<3>(0b111), 0, std::time(nullptr), std::time(nullptr)) {}
    FileDesc(std::bitset<3> permissions) : FileDesc(permissions, 0, std::time(nullptr), std::time(nullptr)) {}
    FileDesc(std::bitset<3> permissions, size_t size) : FileDesc(permissions, size, std::time(nullptr), std::time(nullptr)) {}
    FileDesc(std::bitset<3> permissions, size_t size, const std::time_t &creation_time, const std::time_t &modification_time);



    ~FileDesc() override = default;

    EntityDto info() override;
};


#endif //INC_3_FILEDESC_H