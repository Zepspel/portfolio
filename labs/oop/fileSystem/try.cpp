//
// Created by pavel on 18.12.2025.
//

#include "Service/fs.h"
#include "View/view.h"
#include "Controller/controller.h"

#include <iostream>

int main() {

    Fs fs;
    fs.mkdir("/dir1");
    fs.touch("/dir1/file1", "7");
    fs.touch("/dir1/file2");
    fs.mkdir("/dir1/dir");



    Controller controller(fs);
    ConsoleView console(controller);
    console.run();

    return 0;
}