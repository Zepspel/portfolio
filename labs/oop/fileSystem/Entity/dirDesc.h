//
// Created by pavel on 11.12.2025.
//

#ifndef INC_3_DIRDESC_H
#define INC_3_DIRDESC_H

#include <memory>

#include "desc.h"
#include "spDesc.h"
#include "../Table/orderedTable.h"

//#include "fs.h"

using Key = std::string;
using Value = Desc*;
using Table = OrderedTable<Key,Value>;

constexpr size_t SIZE_4K = 4096;

class Fs;
class FsTest;

/**
 * the repository class that contains container with other entities
 */
class DirDesc: public Desc {
    std::unique_ptr<Table> table;

    friend Fs;
    friend FsTest;
public:
    DirDesc();
    DirDesc(std::bitset<3> permissions);
    DirDesc(std::bitset<3> permissions, size_t size);

    /**
     * @brief recursively delete files and directories within itself
     */
    ~DirDesc() override;

    EntityDto info() override;

    void include_device(const std::string_view& name, std::bitset<3> permissions, size_t size, Mode mode);
    void include_file(const std::string_view& name, std::bitset<3> permissions, size_t size, const std::time_t &creation_time, const std::time_t &modification_time);
    void include_dir(const std::string_view& name, std::bitset<3> permissions, size_t size);
};



#endif //INC_3_DIRDESC_H