//
// Created by pavel on 12.01.2026.
//

#ifndef INC_3_CONTROLLER_H
#define INC_3_CONTROLLER_H

#include <optional>

#include "../View/commandDto.h"
#include "../Service/fs.h"
#include "entityDto.h"


/**
 * controller - transport layer of application
 */
class IController {
public:
    virtual ~IController() = default;

    /**
     * @brief entry point of controller
     * @param cmd command dto
     * @return vector<EntityDto> or nothing
     */
    virtual std::optional<std::vector<EntityDto>> execute(const CommandDTO& cmd) = 0;

protected:
    /**
     * @brief change permission for entity
     */
    virtual void handleChmod(const CommandDTO&) = 0;
    /**
     * @brief get information about entity or entities in particular directory
     * @return vector describing entities
     */
    virtual std::vector<EntityDto> handleLs(const CommandDTO&) = 0;
    /**
     * @brief create new file or device (required flag "-d") in existing directory
     */
    virtual void handleTouch(const CommandDTO&) = 0;
    /**
     * @brief remove file, device or directory (required flag "-r")
     */
    virtual void handleRm(const CommandDTO&) = 0;
    /**
     * @brief create new directory in existing directory
     */
    virtual void handleMkdir(const CommandDTO&) = 0;
    /**
     * @brief change size of entity in memory
     */
    virtual void handleTruncate(const CommandDTO&) = 0;

    /**
     * @brief upload condition of system to the file
     */
    virtual void handleUpload(const CommandDTO&) = 0;

    /**
     * @brief download condition of system from the file
     */
    virtual void handleDownload(const CommandDTO&) = 0;

    /**
     * @brief find all files with relative file name
     * @details has multithreaded mode (flag -t)
     * @param dto
     * @return
     */
    virtual std::vector<EntityDto> handleFind(const CommandDTO& dto) = 0;



    /**
     * @brief handle unknown command
     */
    virtual void handleUnknown(const CommandDTO&) = 0;
};


/**
 * for catch2 tests
 */
class MockController : public IController {
public:
    bool called = false;
    CommandDTO lastDto;

    std::optional<std::vector<EntityDto>> execute(const CommandDTO& dto) override {
        called = true;
        lastDto = dto;
        return std::nullopt;
    }

    // Внутренние операции, доступные только наследникам:
    void handleChmod(const CommandDTO&) override {}
    std::vector<EntityDto> handleLs(const CommandDTO&) override {std::vector<EntityDto> a; return a;}
    void handleTouch(const CommandDTO&) override {}
    void handleRm(const CommandDTO&) override {}
    void handleMkdir(const CommandDTO&) override {}
    void handleTruncate(const CommandDTO&) override {}
    void handleUpload(const CommandDTO&) override {}
    void handleDownload(const CommandDTO&) override {}
    std::vector<EntityDto> handleFind(const CommandDTO& dto) {std::vector<EntityDto> a; return a;}


    void handleUnknown(const CommandDTO&) override {}
};




class Controller : public IController {
    Fs& service;

    std::time_t parseTime(const std::string& str);
public:
    explicit Controller(Fs& service) : service(service) {}


    std::optional<std::vector<EntityDto>> execute(const CommandDTO& cmd) override;

protected:
    void handleTruncate(const CommandDTO& dto) override {
        service.truncate(dto.path, dto.size);
    }

    void handleChmod(const CommandDTO& dto) override {
        service.chmod(dto.path, dto.permissions);
    }

    /**
     * @brief supports flag -l
     * @param dto
     */
    std::vector<EntityDto> handleLs(const CommandDTO& dto) override {
        std::vector<EntityDto> res;
        if (dto.flags.contains("l")) {
            res = service.info(dto.path);
        }
        else {
            res = service.getNames(dto.path);
        }
        return res;
    }

    std::vector<EntityDto> handleFind(const CommandDTO& dto) override {
        std::vector<EntityDto> res;
        if (!dto.flags.empty() && !dto.flags.contains("t")) {
            throw std::invalid_argument("[Controller] Unknown flag");
        }
        if (dto.flags.contains("t")) {
            res = service.find_threads(dto.file);
        }
        else {
            res = service.find(dto.file);
        }
        return res;
    }

    void handleTouch(const CommandDTO& dto) override {
        if (!dto.flags.empty() && !dto.flags.contains("d")) {
            throw std::invalid_argument("[Controller] Unknown flag");
        }
        if (dto.flags.contains("d")) {
            service.device(dto.path);
        }
        else {
            service.touch(dto.path);
        }
    }

    /**
     * @brief supports flag -r
     * cannot remove root
     * @param dto
     */
    void handleRm(const CommandDTO& dto) override {
        if (!dto.flags.empty() && !dto.flags.contains("r")) {
            throw std::invalid_argument("[Controller] Unknown flag");
        }
        if (dto.flags.contains("r")) {
            service.rm_r(dto.path);
        }
        else {
            service.rm(dto.path);
        }
    }

    void handleMkdir(const CommandDTO& dto) override {
        service.mkdir(dto.path);
    }

    void handleUpload(const CommandDTO& dto) override {
        service.upload(dto.file);
    }

    void handleDownload(const CommandDTO& dto) override {
        service.download(dto.file);
    }

    void handleUnknown(const CommandDTO&) override {
        throw std::runtime_error("Unknown command");
    }
};

#endif //INC_3_CONTROLLER_H