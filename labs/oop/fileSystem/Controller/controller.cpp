//
// Created by pavel on 12.01.2026.
//


#include <sstream>
#include <ctime>


#include "controller.h"
#include "entityDto.h"

#include <iomanip>


std::optional<std::vector<EntityDto>> Controller::execute(const CommandDTO& cmd) {
    std::vector<EntityDto> res;

    switch (cmd.type) {
    case CommandType::CHMOD:  handleChmod(cmd); break;
    case CommandType::LS:     res = handleLs(cmd); break;
    case CommandType::TOUCH:  handleTouch(cmd); break;
    case CommandType::MKDIR:  handleMkdir(cmd); break;
    case CommandType::TRUNCATE: handleTruncate(cmd); break;
    case CommandType::RM:       handleRm(cmd); break;
    case CommandType::UPLOAD:   handleUpload(cmd); break;
    case CommandType::DOWNLOAD: handleDownload(cmd); break;
    case CommandType::FIND: res = handleFind(cmd); break;
    default: handleUnknown(cmd); break;
    }
    return res;
}

std::time_t Controller::parseTime(const std::string& str) {
    std::tm tm = {};
    std::istringstream ss(str);

    // формат строки
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse time string");
    }

    // преобразовать tm → time_t
    return std::mktime(&tm);
}



/*
ОСТАНОВИЛСЯ НА ЭТОМ МОМЕНТЕ, НАДО ЕЩЕ РАЗ ПРОДУМАТЬ КАК ФУНКЦИЯ ЭТА РАБОТАТЬ БУДЕТ
void Controller::handleTouch(const CommandDTO& dto) {

    //touch -t ВРЕМЯ file
    auto it = dto.flags.find("t");
    if (it != dto.flags.end()) {
        try {
            service.changeModificationTime(dto.path, parseTime(it->second)); //если такого файла не существует, то создадим новый
        } catch (std::exception&) {
            auto desc = mapper.mapFromDto(dto.entity);      //пусть маппер только делает entity исходя из описания
        }
    }


    service.createFile(dto.path);
}*/


