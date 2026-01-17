//
// Created by pavel on 12.01.2026.
//

#include "view.h"


std::vector<std::string> ConsoleView::parse(const std::string &input) {
    std::vector<std::string> words;
    size_t start = input.find_first_not_of(" \t");

    while (start != std::string::npos) {
        auto pos = input.find_first_of(" \t", start);


        words.push_back(input.substr(start, pos - start));
        start = input.find_first_not_of(" \t", pos);
    }
    return words;
}



CommandDTO ConsoleView::makeDTO(const std::string& input) {
    std::vector<std::string> words = parse(input);

    if (words.empty()) {
        throw std::invalid_argument("Empty command");
    }

    CommandDTO command;

    const std::string& cmd = words[0];
    if (cmd == "ls") {
        if ((words.size() != 2) && (words.size() != 3)) {
            throw std::invalid_argument("Too many arguments");
        }
        command.type = CommandType::LS;
    }
    else if (cmd == "touch") {
        if ((words.size() != 2) && (words.size() != 3))  {
            throw std::invalid_argument("Too many arguments");
        }
        command.type = CommandType::TOUCH;
    }
    else if (cmd == "mkdir") {
        if (words.size() != 2)  {
            throw std::invalid_argument("Too many arguments");
        }
        command.type = CommandType::MKDIR;
    }
    else if (cmd == "rm") {
        if ((words.size() != 2) && (words.size() != 3)) {
            throw std::invalid_argument("Too many arguments");
        }
        command.type = CommandType::RM;
    }
    else if (cmd == "chmod") {
        if (words.size() != 3) {
            throw std::invalid_argument("Too many arguments");
        }
        command.type = CommandType::CHMOD;
    }
    else if (cmd == "truncate") {
        if (words.size() != 3) {
            throw std::invalid_argument("Too many arguments");
        }
        command.type = CommandType::TRUNCATE;
    }
    else if (cmd == "upload") {
        if (words.size() != 2) {
            throw std::invalid_argument("Too many arguments");
        }
        command.type = CommandType::UPLOAD;
    }
    else if (cmd == "download") {
        if (words.size() != 2) {
            throw std::invalid_argument("Too many arguments");
        }
        command.type = CommandType::DOWNLOAD;
    }
    else if (cmd == "find") {
        if ((words.size() != 2) && (words.size() != 3)) {
            throw std::invalid_argument("Too many arguments");
        }
        command.type = CommandType::FIND;
    }
    else
        throw std::invalid_argument("Unknown command: " + cmd);


    bool pathSet = false;

    // Разбираем аргументы
    for (size_t i = 1; i < words.size(); ++i) {
        const std::string& w = words[i];

        if (!w.empty() && w[0] == '-') {
            // Flags: remove the initial '-'
            command.flags.insert(w.substr(1));
        }
        else if ((!pathSet) && (w[0] == '/')) {
            // Первый аргумент с '/' — путь
            command.path = w;
            pathSet = true;
        }
        else if (command.type == CommandType::CHMOD) {
            // For chmod, the following arguments after the path are permissions
            command.permissions = w;
        }
        else if (command.type == CommandType::TRUNCATE) {
            command.size = std::stoi(w);
        }
        else if ((command.type == CommandType::UPLOAD) || (command.type == CommandType::DOWNLOAD) || (command.type == CommandType::FIND)) {
            command.file = w;
        }
        else {
            throw std::invalid_argument("Unexpected argument: " + w);
        }
    }

    // Checking required fields
    if (((command.type != CommandType::UPLOAD) && (command.type != CommandType::DOWNLOAD) && (command.type != CommandType::FIND))  && command.path.empty()) {
        throw std::invalid_argument("Path is required for command: " + cmd);
    }

    return command;
}


void ConsoleView::run() {
    while (true) {
        try {
            std::string input = readInput();
            if ((input == "exit") || (input == ""))
                break;

            CommandDTO command = makeDTO(input);

            auto dtos = controller.execute(command);
            if (dtos) {
                showDtos(*dtos);
            }
        } catch (const std::exception& e) {
            showError(e.what());
        }
    }
}

std::string ConsoleView::readInput() {
    std::cout << "> ";
    std::string line;
    std::getline(std::cin, line);
    return line;
}



