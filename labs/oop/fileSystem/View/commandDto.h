//
// Created by pavel on 12.01.2026.
//

#ifndef INC_3_COMMANDDTO_H
#define INC_3_COMMANDDTO_H


#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>


enum class CommandType : char { TOUCH, MKDIR, CHMOD, TRUNCATE, RM, LS, UPLOAD, DOWNLOAD, FIND };

/**
 * structure describing command
 * it is formed by view and passed to controller
 * it has:
 * command type
 * flags (if they exist)
 * path to the entity witch we work with
 * new permissions for that entity (if required)
 * new size for that entity (if required)
 *
 * the commands and the fields they use
 * upload/download: file
 * ls: path, flags
 * touch: path
 * mkdir: path
 * rm: path, flags
 * chmod: path, permissions
 * truncate: path, size
 * find: file
 */
struct CommandDTO {
    CommandType type;
    std::unordered_set<std::string> flags;

    std::string path;

    std::string permissions;
    size_t size;

    std::string file;
};


#endif //INC_3_COMMANDDTO_H