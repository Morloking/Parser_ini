#include <iostream>
#include <string>
#include <clocale>
#include <memory>
#include "parser.h"

int main() {
    setlocale(LC_ALL, "RU");
    try {
        core::Parser parser("input.ini");
        std::cout << "Section1.var1 = " << parser.getValue<double>("Section1.var1") << "\n";
        std::cout << "Section1.var2 = " << parser.getValue<std::string>("Section1.var2") << "\n";
        std::cout << "Section1.var3 = " << parser.getValue<std::string>("Section1.var3") << "\n";
        std::cout << "Section2.var1 = " << parser.getValue<int>("Section2.var1") << "\n";
        std::cout << "Section2.var2 = " << parser.getValue<std::string>("Section2.var2") << "\n";
        std::cout << "Section4.Mode = '" << parser.getValue<std::string>("Section4.Mode") << "'\n";
        std::cout << "Section4.Vid  = '" << parser.getValue<std::string>("Section4.Vid") << "'\n";
        // parser.getValue<int>("Section1.Nothing");
        // parser.getValue<int>("Nothing.var1");
        // parser.getValue<int>("Section3.any");
    }
    catch (const std::exception& exc) {
        std::cerr << "Error: " << exc.what() << "\n";
        return 1;
    }
    return 0;
}