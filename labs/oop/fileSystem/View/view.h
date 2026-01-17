//
// Created by pavel on 12.01.2026.
//

#ifndef INC_3_VIEW_H
#define INC_3_VIEW_H


#include <string>
#include <iostream>

#include "commandDto.h"
#include "Controller/controller.h"
#include "../Controller/entityDto.h"


/**
 * layer for interaction with user
 */
class IView {
protected:
    /**
     * @brief show available information about files by dtos
     * @param dtos the structure describing files
     */
    virtual void showDtos(const std::vector<EntityDto>& dtos) = 0;

    /**
    * @brief show message to user
    * @param text message
    */
    virtual void showMessage(const std::string& text) = 0;

    /**
     * @brief show exception message to user
     * @param msg error message
     */
    virtual void showError(const std::string& msg) = 0;

    /**
     * @brief get input string
     * @return string
     */
    virtual std::string readInput() = 0;

    /**
     * @brief make command dto
     * @param input string
     * @return command dto
     */
    virtual CommandDTO makeDTO(const std::string& input) = 0;
public:
    virtual ~IView() = default;

    /**
     * @brief dialog cycle
     */
    virtual void run() = 0;
};


/**
 * this view getting and printing information from the console
 * has link to the controller that has connection with service and selects appropriate methods from it
 */
class ConsoleView : public IView {
    Controller &controller;

    /**
     * @brief split input string into separate words
     * @param input
     * @return
     */
    std::vector<std::string> parse(const std::string &input);
    CommandDTO makeDTO(const std::string& input) override;

    std::string readInput() override;

    void showMessage(const std::string& text) override {
        std::cout << text << std::endl;
    }

    void showDtos(const std::vector<EntityDto>& dtos) override {
        for (auto dto: dtos) {
            std::cout << dto << std::endl;
        }
    }

    void showError(const std::string& msg) override {
        std::cerr << "Error: " << msg << std::endl;
    }
public:
    ConsoleView(Controller &controller) : controller(controller) {}

    void run() override;
};



/**
 * for catch2 testing
 * be able to pass inputs to the run()
 */
class TestConsoleView : public ConsoleView {
    std::vector<std::string> inputs;
    size_t index = 0;

public:
    TestConsoleView(Controller &controller, std::vector<std::string> in)
        : ConsoleView(controller), inputs(std::move(in)) {}

protected:
    std::string readInput() override {
        return inputs[index++];
    }
};



#endif //INC_3_VIEW_H