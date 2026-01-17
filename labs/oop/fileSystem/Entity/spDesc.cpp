//
// Created by pavel on 11.12.2025.
//

#include "spDesc.h"

std::string to_string(Mode m) {
    switch (m) {
    case Mode::Block: return "Block";
    case Mode::Character: return "Character";
    }
    return "Unknown";
}



EntityDto SpDesc::info() {
    auto info = Desc::info();


    info.mode = to_string(mode);

    return info;
}
