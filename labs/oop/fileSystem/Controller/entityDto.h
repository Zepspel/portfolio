//
// Created by pavel on 12.01.2026.
//

#ifndef INC_3_ENTITYDTO_H
#define INC_3_ENTITYDTO_H

#include <iomanip>
#include <string>
#include <ostream>

/**
 * structure describing entity
 * it's formed by service with entities help and passed to controller, he forwards it to the view and view shows it to user
 * not all fields are required to be filled in
 */
struct EntityDto {
    std::string type;
    std::string permissions;
    std::string size;
    std::string addr;
    std::string creation_time;
    std::string modification_time;
    std::string count;
    std::string mode;
    std::string name;
    std::string path;
};



inline std::ostream& operator<<(std::ostream& os, const EntityDto& dto) {

    os << std::left;
    if (dto.permissions != "") {
       os << std::setw(4)  << dto.permissions;
    }
    if (dto.size != "") {
        os << std::setw(8) << dto.size;
    }
    if (dto.addr != "") {
        os << std::setw(16) << dto.addr;
    }
    if (dto.creation_time != "") {
        os << std::setw(20) << dto.creation_time;
    }
    if (dto.modification_time != "") {
        os << std::setw(20) << dto.modification_time;
    }
    if (dto.mode != "") {
        os << std::setw(10) << dto.mode;
    }
    if (dto.name != "") {
        os << std::setw(20) << dto.name;
    }
    if (dto.path != "") {
        os << std::setw(16) << dto.path;
    }


    return os;
}





#endif //INC_3_ENTITYDTO_H