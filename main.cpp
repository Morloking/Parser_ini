#include <iostream>
#include <string>
#include <clocale>

#include "parser.h"

int main() {
    setlocale(LC_ALL, "RU");
    try {
        core::Parser p("input.ini");
        std::cout << "Section1.var1 = " << p.getValue<double>("Section1.var1") << "\n";
        std::cout << "Section1.var2 = " << p.getValue<std::string>("Section1.var2") << "\n";
        std::cout << "Section1.var3 = " << p.getValue<std::string>("Section1.var3") << "\n";
        std::cout << "Section2.var1 = " << p.getValue<int>("Section2.var1") << "\n";
        std::cout << "Section2.var2 = " << p.getValue<std::string>("Section2.var2") << "\n";
        std::cout << "Section4.Mode = '" << p.getValue<std::string>("Section4.Mode") << "'\n";
        std::cout << "Section4.Vid  = '" << p.getValue<std::string>("Section4.Vid") << "'\n";
        // p.getValue<int>("Section1.Nothing");
        // p.getValue<int>("Nothing.var1");
        // p.getValue<int>("Section3.any");
    }
    catch (const std::exception& exc) {
        std::cerr << "Error: " << exc.what() << "\n";
        return 1;
    }
    return 0;
}