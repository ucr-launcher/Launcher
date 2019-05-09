//********************************************************//
//*                   A series of utilies                *//
//*					     By Aaron Sigal                  *//
//********************************************************//

#include <iostream>
#include <string>

// A clean and uniform way to print errors to the console without acually throwing an exception
void error(std::string message) {
	cout << "[ERROR]: " + message << endl;
}

// A clean and uniform way to print warnings to the console without actually throwing an exception
void warn(std::string message) {
	cout << "[WARN]: " + message << endl;
}

