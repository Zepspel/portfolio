//
// Created by pavel on 21.11.2025.
//

#include <unordered_map>
#include <sstream>

#include <iostream>

#include "desc.h"

#include <cstdint>


/*std::string to_string(Type t) {
    switch (t) {
    case Type::File: return "File";
    case Type::Directory: return "Directory";
    case Type::Device: return "Device";
    default: return "Unknown";
    }
}*/


Desc::~Desc() {
    if (addr)
        delete [] addr;
}

void Desc::setPermissions(std::bitset<3> permissions) {
    this->permissions = permissions;
}

void Desc::setSize(size_t size) {
    size_ = size;
    delete[] addr;
    addr = new char[size_];
}

EntityDto Desc::info() {
    EntityDto info;


    info.permissions = to_string(permissions);
    info.size = std::to_string(size_);

    std::ostringstream oss;


    oss << std::hex << std::showbase
    << reinterpret_cast<uintptr_t>(addr);


    info.addr = oss.str();

    return info;
}


std::string to_string(std::bitset<3> permissions) {
    std::string str;
    if (permissions.test(2))
        str += 'r';
    else
        str += '-';

    if (permissions.test(1))
        str += 'w';
    else
        str += '-';

    if (permissions.test(0))
        str += 'x';
    else
        str += '-';

    return str;
}


