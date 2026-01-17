#include <iostream>
#include <cstring>
#include <algorithm>
#include "crossword.h"

using std::cin;
using std::cout;
using std::cerr;
using std::vector;
using std::string;
using std::endl;

const vector<string> words = {"app", "apply", "apple", "apo", "bear", "applere", ""," ", "8", "apple"};

vector<string> crossword(const string& template_word)
{
	vector<string> ans;

	for (const string& word: words) {
		if (std::equal(template_word.begin(), template_word.end(), word.begin(), word.end(), comparison))
			ans.push_back(word);
	}

	return ans;
}

bool comparison(const char& a, const char& b)
{
	if (std::isspace(a))
		throw std::invalid_argument("Space are not allowed");

	if ((a == b) || (a == '?'))
		return true;

	return false;
}

const char** crossword(const char *cstr) {
    string str;
    vector<string> ans = crossword(string(cstr));

    const char** list = new const char*[ans.size() + 1];
    size_t i = 0;
    const size_t len = strlen(cstr);

    for (string &word: ans) {
	    char* elem = new char[len + 1];
    	std::copy(word.begin(), word.end(), elem);
    	elem[len] = '\0';
        list[i++] = elem;
    }
    list[i] = nullptr;

    return list;
}


