//
// Created by pavel on 07.10.2025.
//

#ifndef INC_2_DIALOG_H
#define INC_2_DIALOG_H

void dialog(std::vector<units::Pin> &pins, std::vector<units::Gate> &gates);
void help();
void leave(bool &work);
void create(std::vector<units::Pin> &pins, std::vector<units::Gate> &gates);
void erase(std::vector<units::Pin> &pins, std::vector<units::Gate> &gates);
void edit(std::vector<units::Pin> &pins, std::vector<units::Gate> &gates);
void output(std::vector<units::Pin> &pins, std::vector<units::Gate> &gates);


#endif //INC_2_DIALOG_H