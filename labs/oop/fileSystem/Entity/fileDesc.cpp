//
// Created by pavel on 11.12.2025.
//

#include "fileDesc.h"






FileDesc::FileDesc(std::bitset<3> permissions, size_t size, const std::time_t &creation_time, const std::time_t &modification_time)
: Desc(permissions, size), creation_time(creation_time), modification_time(modification_time) {
    /*auto now = std::chrono::system_clock::now();                            // текущее время
    creation_time= std::chrono::system_clock::to_time_t(now);                                  // перевод в time_t
    modification_time = creation_time;*/
}

std::string timeToString(std::time_t t) {
    std::tm* tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%b %d %H:%M");
    return oss.str();
}

EntityDto FileDesc::info() {
    auto info = Desc::info();

    info.type = "file";

    info.creation_time = timeToString(creation_time);
    info.modification_time = timeToString(modification_time);

    return info;
}





