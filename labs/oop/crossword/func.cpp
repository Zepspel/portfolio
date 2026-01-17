#include <iostream>
#include <vector>
#include <string>
#include "crossword.h"

using std::cin;
using std::cout;
using std::cerr;
using std::vector;
using std::string;
using std::endl;






void plus_plus(const string& template_string) {

    cout << "C++ style" << endl;

    vector<string> ans = crossword(template_string);

    for (const string& word: ans) {
        cout << word << '\n';
    }

    if (ans.empty()) {
        cout << "There is no answer" << endl;
    }
}



void no_plus(const char *template_word) {
   
    cout << "C style" << endl;
    const char **list = crossword(template_word);

    size_t i = 0;
    if (list[i] == nullptr) {
        cout << "There is no answer" << endl;
    }

    for (i = 0; list[i] != nullptr; i++) {
        cout << list[i] << '\n';
    }


    for (i = 0; list[i] != nullptr; i++) {
        delete [] list[i];
    }

    delete [] list;
}


