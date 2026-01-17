//
// Created by pavel on 11.12.2025.
//

#include "dirDesc.h"

#include <ranges>

#include "fileDesc.h"


DirDesc::DirDesc() : Desc(std::bitset<3>(0b111), SIZE_4K) {
    table = std::make_unique<Table>();
}


DirDesc::DirDesc(std::bitset<3> permissions) : Desc(permissions, SIZE_4K) {
    table = std::make_unique<Table>();
}

DirDesc::DirDesc(std::bitset<3> permissions, size_t size) : Desc(permissions, size) {
    table = std::make_unique<Table>();
}


DirDesc::~DirDesc() {
    //должны удалить рекурсивно все файлы, которые лежат в директории

    if (!table) return;

    for (auto& desc : *table | std::views::values) {
        if (desc)
            delete desc;
    }
    //из таблицы не удаляем их, так как удалится сама таблица (за ней память чистить не нужно, так как она на unique ptr)
}



EntityDto DirDesc::info() {
    auto info = Desc::info();

    /*//нужно сделать список со всеми именами файлов в таблице
    std::string names;
    for (auto it = table->begin(); it != table->end(); ++it) {
        names += it->first + "\n";
    }*/


    info.count = std::to_string(table->size());

    return info;
}

void DirDesc::include_device(const std::string_view& name, std::bitset<3> permissions, size_t size, Mode mode) {
    if (!this->permissions.test(1)) {
        throw std::logic_error("Permission denied: no write permission in parent directory");
    }


    auto dev_desc = new SpDesc(permissions, size, mode);

    table->insert(std::make_pair(std::string(name), dev_desc));
}


void DirDesc::include_file(const std::string_view& name, std::bitset<3> permissions, size_t size, const std::time_t &creation_time, const std::time_t &modification_time) {
    // Проверяем права на запись в директории
    if (!this->permissions.test(1)) {
        throw std::logic_error("Permission denied: no write permission in parent directory");
    }



    auto file_desc = new FileDesc(permissions, size, creation_time, modification_time);

    //table->operator[](std::string(name)) = file_desc;

    table->insert(std::make_pair(std::string(name), file_desc));
}


void DirDesc::include_dir(const std::string_view& name, std::bitset<3> permissions, size_t size) {
    // Проверяем права на запись в директорию
    if (!this->permissions.test(1)) {
        throw std::logic_error("Permission denied: no write permission in parent directory");
    }

    auto dir_desc = new DirDesc(permissions, size);

    //table->operator[](std::string(name)) = dir_desc;

    table->insert(std::make_pair(std::string(name), dir_desc));

}








