#ifndef _CROSSWORD_H_
#define _CROSSWORD_H_

#include <vector>


std::vector<std::string> crossword(const std::string& template_word);
const char** crossword(const char *template_word);

bool comparison(const char& a, const char& b);




#endif
