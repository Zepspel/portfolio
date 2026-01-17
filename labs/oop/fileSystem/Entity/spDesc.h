//
// Created by pavel on 11.12.2025.
//

#ifndef INC_3_SPDESC_H
#define INC_3_SPDESC_H

#include "desc.h"

enum class Mode : unsigned char { Character, Block };

/**
 * descriptor of device
 */
class SpDesc: public Desc {

public:
    Mode mode;

    SpDesc(std::bitset<3> permissions, size_t size, Mode mode) : Desc(permissions, size) {this->mode = mode;}
    ~SpDesc() override = default;


    EntityDto info() override;
};



#endif //INC_3_SPDESC_H