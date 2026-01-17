#include <iostream>
#include <string>
#include "func.h"

using std::cin;
using std::cout;
using std::cerr;
using std::string;
using std::endl;

int main()
{

	string template_string;


    cout << "Enter template\n";
    getline(cin, template_string);

    
    try {
	    plus_plus(template_string);
    } 
    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        //return 1;
    }
	

    try {
        no_plus(template_string.data());
    }
    catch (const std::exception& e) {
	cerr << "Error: " << e.what() << endl;
        return 1;
    }

   
	return 0;
}


