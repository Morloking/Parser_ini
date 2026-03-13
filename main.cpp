#include <string>

#include "ini_parser.h"

int main() {
    setlocale(LC_ALL, "Russian");
    try {
	    ini_parser::INIParser pars("file1.ini");
	    auto value = pars.getValue <double>("Section3.var1");
    }
    catch (const std::invalid_argument& errorInputFunction) {
	    std::cerr << "Îøèáêà: " << errorInputFunction.what() << "\n";
    }
    catch (const std::runtime_error& errorInputFile) {
	    std::cerr << "Îøèáêà: " << errorInputFile.what() << "\n";
    }
}