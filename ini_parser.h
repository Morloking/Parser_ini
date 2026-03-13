#pragma once

#include <string>
#include <type_traits>
#include <algorithm>
#include <stdexcept>
#include <map>

namespace ini_parser {

template<typename> struct always_false : std::false_type {};

class INIParser {
public: 
    explicit INIParser(const std::string& filename);

    bool deleteSpacesAndTabs(std::string& rawString);

    void initializeFileData();

    template <typename T>
    T getValue(const std::string& sectionAndVarName) {


	    //поиск индекса разделителя '.'
	    size_t indexSeparator{ 0 };
	    auto resultFind = std::find(begin(sectionAndVarName), end(sectionAndVarName), '.');
	    if (resultFind == end(sectionAndVarName)) {
		    throw std::invalid_argument("Название секции и название переменной некорректны. Используйте формат: название секции. название переменной");
	    }
	    indexSeparator = resultFind - begin(sectionAndVarName);
	    //получение имени секции и название переменной
	    std::string sectionName = sectionAndVarName.substr(0, indexSeparator);
	    std::string varName = sectionAndVarName.substr(indexSeparator + 1); //вывести всё после точки
        auto sectionIt = fileData.find(sectionName);
	    if (sectionIt != fileData.end()) {
		    auto varIt = sectionIt->second.find(varName);
		    if (varIt != sectionIt->second.end()) {
			    if constexpr (std::is_same_v <T, int>)
				    return std::stoi(varIt->second);
			    else if constexpr (std::is_same_v <T, double >)
				    return std::stod(varIt->second);
			    else if constexpr (std::is_same_v <T, std::string>)
				    return varIt->second;
			    else {
				    static_assert(always_false<T>::value, "Неподдерживаемый тип");
			    }
		    }
		    else {
			    std::string tempHint = getHint(sectionName,'v');
			    if (tempHint.empty()) throw std::runtime_error ("Искомого значения нет. В секции отсутствуют переменные.");
			    else throw std::runtime_error ("Искомого значения нет. Подсказка(названия других переменных в секции): " + tempHint);
		    }

	    }
	    else throw std::runtime_error("Искомой секции нет. Доступные секции: " + getHint("", 's'));
    }

    std::string getHint(std::string sectionName, char mode);
	
private:
    const std::string filename{ "" };
    std::map <std::string, std::map <std::string, std::string>> fileData{};
	
};

}


