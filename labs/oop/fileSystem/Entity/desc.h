//
// Created by pavel on 21.11.2025.
//

#ifndef INC_3_DESC_H
#define INC_3_DESC_H

#include <bitset>
#include <list>



#include "../Controller/entityDto.h"



enum class Type : unsigned char { File, Directory, Device };

std::string to_string(Type t);



/**
* the base entity class representing common descriptors
*/
class Desc {
protected:
    std::bitset<3> permissions;
    size_t size_;
    char *addr;

    public:
    Desc() : permissions(std::bitset<3>(0b111)), size_(0), addr(nullptr) {}
    Desc(std::bitset<3> permissions, size_t size) : permissions(permissions), size_(size), addr(new char[size]) {}

    virtual ~Desc();

    //методы с одинаковой реализацией для всех наследников
    std::bitset<3> getPermissions() const {return permissions;};
    void setPermissions(std::bitset<3> permissions);

    size_t getSize() const {return size_;};
    void setSize(size_t size);

    const char *getAddr() const {return addr;};

    /**
     * smart entity writes its own properties to dto
     * @return dto describing descriptor
     */
    virtual EntityDto info();
};

std::string to_string(std::bitset<3> permissions);







#endif //INC_3_DESC_H