#include "ini_parser.h"

#include <fstream>

namespace ini_parser {
    INIParser::INIParser(const std::string& filename) : filename(filename){
		initializeFileData();
    }

    bool INIParser::deleteSpacesAndTabs(std::string& rawString) {
    size_t startSpaces = rawString.find_first_not_of(" \t");
	if (startSpaces == std::string::npos) return false;
    size_t endSpaces = rawString.find_last_not_of(" \t");
    if (endSpaces == std::string::npos) return false;
    rawString = rawString.substr(startSpaces, endSpaces - startSpaces + 1);
    return true; //если строка после удаления лишнего не стала пустой 
}

void INIParser::initializeFileData() {
    int counterLines{ 0 };
    std::string line;
    std::string currNameSection{ "" };
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Не удалось открыть файл");

    while (std::getline(file, line)) {
	    ++counterLines;
	    //0. отбрасывание комментариев ';'
	    size_t resultFindComm = line.find(';');
	    if (resultFindComm != std::string::npos)
		    line = line.substr(0, resultFindComm);
	    if (!deleteSpacesAndTabs(line)) continue;

	    //1. поиск  и запись названия секции, если это секция

	    size_t resultBeginSection = line.find('[');
	    size_t resultEndSection = line.find(']');
	    if (resultBeginSection != std::string::npos && resultEndSection != std::string::npos) {
		    currNameSection = line.substr(resultBeginSection + 1, resultEndSection - resultBeginSection - 1);
		    if (!deleteSpacesAndTabs(currNameSection)) throw std::runtime_error("Пустое имя секции");
		    fileData[currNameSection];
		    continue;
	    }
	    //2. Поиск и запись названия переменной в секции	
	    if (currNameSection.empty()) throw std::runtime_error("Переменная вне секции");

	    size_t resultEndNameVar = line.find('=');
	    if (resultEndNameVar != std::string::npos) {
		    std::string tempName = line.substr(0, resultEndNameVar);

		    //3. Запись значения переменной в секции
		    std::string tempValue = line.substr(resultEndNameVar + 1);
		    //очистили строки от пустых пространств
		    if (!deleteSpacesAndTabs(tempName)) throw std::runtime_error("Пустое имя переменной");
		    deleteSpacesAndTabs(tempValue);
		    fileData[currNameSection][tempName] = tempValue;
	    }
	    else throw std::runtime_error("Некорректный синтаксис строки: " + std::to_string(counterLines));
    }
}

std::string INIParser::getHint(std::string sectionName, char mode) { // подсказка для: s - секции, v  - названия переменных
    std::string hint{ "" };
    if (mode == 'v') {
	    auto varIt = fileData.find(sectionName);
	    if (varIt != fileData.end()) {
		    const auto& innerMap = varIt->second;
		    for (const auto& elem : innerMap) {
			    hint += elem.first;
			    hint += ", ";
		    }
			if (!hint.empty()) { //удаляем последний пробел и запятую
			    hint.pop_back();
			    hint.pop_back();
		    }
	    }
    }

    if (mode == 's') {
	    for (const auto& elem : fileData) {
		    hint += elem.first;
		    hint += ", ";
	    }
	    if (!hint.empty()) { //удаляем последний пробел и запятую
		    hint.pop_back();
		    hint.pop_back();
	    }
    }
    return hint;
}

}
