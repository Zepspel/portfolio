#include <any>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <exception>

#include "gate.h"
#include "dialog.h"

#include <limits>


void help() {
    const std::string str = "Options:\n1) Create\n2) Erase\n3) Edit\n4) Help\n5) Output\n6) Leave\n";
    std::cout << str;
}

void leave(bool &work) {
    work = false;
}


void create(std::vector<units::Pin> &pins, std::vector<units::Gate> &gates) {
    std::string str;
    std::cout << "What do you want to create Pin(1) or Gate(2) ?\n";
    std::cin >> str;

    if (str == "Pin" || str == "pin" || str == "1") {
        std::cout << "Enter Type, State, Number of connection separated by a space\n";
        units::Pin p;
        std::cin >> p;
        pins.push_back(p);
        std::cout << "Pin created successfully\n" << std::endl;
    }
    else if (str == "Gate" || str == "gate" || str == "2") {
        std::cout << "Enter number of input and output pins" << std::endl;
        units::Gate g;
        std::cin >> g;
        gates.push_back(g);
        std::cout << "Gate created successfully\n" << std::endl;
    }
    else {
        std::cerr << "Unknown option: " << str << std::endl;
    }
}



void erase(std::vector<units::Pin> &pins, std::vector<units::Gate> &gates) {
    std::string type;
    std::cin >> type;

    unsigned number;
    std::cout << "Which one?\n";
    std::cin >> number;

    if (std::cin.fail()) {
        std::cout << "Error! Not a number entered\n";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }


    if (type == "Pin" || type == "pin" || type == "1") {
        if (number > pins.size() || number == 0) {
            std::cout << "No pin with such number\n";
            return;
        }
        pins.erase(pins.begin() + number - 1);
        std::cout << "Pin erased successfully\n" << std::endl;
    }
    else if (type == "Gate" || type == "gate" || type == "2") {
        if (number > gates.size() || number == 0) {
            std::cout << "No gate with such number\n";
            return;
        }
        gates.erase(gates.begin() + number - 1);
        std::cout << "Gate erased successfully\n" << std::endl;
    }
    else {
        std::cerr << "Unknown option: " << type << std::endl;
    }
}

void edit(std::vector<units::Pin> &pins, std::vector<units::Gate> &gates) {
    std::cout << "You have entered the edit section. To go back, write leave or ctrl^D\n";

    while (true) {
        std::string type;
        std::cout << "What do you want to edit Pin(1) or Gate(2) ?\n[edit] >> ";
        std::cin >> type;

        if (type == "Leave" || type == "leave") {
            return;
        }

        unsigned number;
        std::cout << "Which one?\n[edit] >> ";
        std::cin >> number;

        if (std::cin.fail()) {
            std::cout << "Error! Not a number entered\n";

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }

        if (type == "Pin" || type == "pin" || type == "1") {
            if (number > pins.size() || number == 0) {
                std::cout << "No pin with such number\n";
                continue;
            }

            std::cout << "Choose one of the following options:\n";
            std::cout << R"(
        1) (++) increase in the number of connections by 1;
        2) (–-) decrease in the number of connections by 1;
        3) (>>) Connection of output terminal and input terminal (status is transferred from output terminal to input terminal);
        4) Disconnecting the output terminal and the input terminal (assuming that the terminals have been connected).
        )" << std::endl;

            std::string option;
            std::cin >> option;
            if (option == "1") {
                ++pins[number];
            }
            else if (option == "2") {
                --pins[number];
            }
            else if (option == "3") {
                std::cout << "With which one do you want to connect\n";
                unsigned number2;
                std::cin >> number2;
                if (std::cin.fail()) {
                    std::cout << "Error! Not a number entered\n";

                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return;
                }
                if (number2 > pins.size() || number2 == 0) {
                    std::cout << "No pin with such number\n";
                    return;
                }

                std::cout << pins[number-1] << pins[number2-1] << std::endl;

                pins[number-1] >> pins[number2-1];
                std::cout << "Pins connected successfully\n";
            }
        }
        else if (type == "Gate" || type == "gate" || type == "2") {
            if (number > gates.size() || number == 0) {
                std::cout << "No gate with such number\n";
                return;
            }
        }
    }
}

void output(std::vector<units::Pin> &pins, std::vector<units::Gate> &gates) {
    std::string str;
    std::cout << "What do you want to output Pins(1) or Gates(2) ?\n";
    std::cin >> str;

    unsigned i = 1;
    if (str == "Pins" || str == "pins" || str == "1") {
        for (auto &p : pins) {
            std::cout << i++ << ")\n";
            std::cout << p;
        }
        if (i == 1) {
            std::cout << "No pins\n";
        }
    }
    else if (str == "Gates" || str == "gates" || str == "2") {
        for (auto &g : gates) {
            std::cout << i++ << ")\n";
            std::cout << g;
        }
        if (i == 1) {
        std::cout << "No gates\n";
        }
    }

    else {
        std::cerr << "Unknown option: " << str << std::endl;
    }
}

void dialog(std::vector<units::Pin> &pins, std::vector<units::Gate> &gates) {

    std::unordered_map<std::string, std::any> functions;


    using Args2 = std::function<void(std::vector<units::Pin>&, std::vector<units::Gate>&)>;
    using Args1 = std::function<void(bool&)>;
    using Args0 = std::function<void()>;


    functions["create"] = std::function(create);
    functions["Create"] = std::function(create);
    functions["1"] = std::function(create);


    functions["erase"] = std::function(erase);
    functions["Erase"] = std::function(erase);
    functions["2"] = std::function(erase);


    functions["edit"] = std::function(edit);
    functions["Edit"] = std::function(edit);
    functions["3"] = std::function(edit);


    functions["help"] = std::function(help);
    functions["Help"] = std::function(help);
    functions["4"] = std::function(help);


    functions["output"] = std::function(output);
    functions["Output"] = std::function(output);
    functions["5"] = std::function(output);


    functions["leave"] = std::function(leave);
    functions["Leave"] = std::function(leave);
    functions["6"] = std::function(leave);


    help();
    bool work = true;

    while (work) {
        std::string option;
        std::cout << ">> ";
        std::cin >> option;
        try {
            if (!functions.contains(option)) {
                std::cout << "Unknown option: " << option << std::endl;
            }
            else {
                auto func_any = functions[option];

                if (func_any.type() == typeid(Args2)) {
                    auto func = std::any_cast<Args2>(func_any);
                    func(pins, gates);
                }
                else if (func_any.type() == typeid(Args1)) {
                    auto func = std::any_cast<Args1>(func_any);
                    func(work);
                }
                else if (func_any.type() == typeid(Args0)) {
                    auto func = std::any_cast<Args0>(func_any);
                    func();
                }
                else {
                    std::cout << "Unknown option: " << option << std::endl;
                }
            }
        }
        catch (std::exception &e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }


    }
}
