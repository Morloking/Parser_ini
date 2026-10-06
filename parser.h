#pragma once
#include <string>
#include <map>
#include <type_traits>
#include <stdexcept>
#include <string_view>
#include <utility>

namespace core {
	namespace consts {
		// general pars
		inline constexpr char commMark = ';';
		inline constexpr char sectionOpen = '[';
		inline constexpr char sectionClose = ']';
		inline constexpr char equalMark = '=';
		inline constexpr char pointMark = '.';
		inline constexpr std::string_view trimmingKit = " \t\r\n\f\v";
		// general var
		inline constexpr std::size_t zero = 0;
		// other
		inline constexpr const char* expectedInputFormat = " expected \"section.value\" format ";
	}
	class Parser { 
	public:
		explicit Parser(std::string filename) :
			filename(std::move(filename)) { 
			loadDataFromFile();
		}
		template <typename T> 
		T getValue(std::string_view rawKey) const {
			const auto& [section, varName] = convertInputStrToKey(rawKey); // key.first - section, key.second - varName
			auto sectionIt = filedata.find(section);
			if (sectionIt == filedata.end())
				throw std::runtime_error("section does not exist: " + section + ". Available sections: " + getAvailableParams(filedata));
			auto varIt = sectionIt->second.find(varName);
			if (varIt == sectionIt->second.end())
				throw std::runtime_error("variable does not exist: " + varName + " in section - " + section + ". Available variables: " + getAvailableParams(sectionIt->second));
			return convertRawValueToFinal<T>(varIt->second);
		}
	private:
		template <typename T>
		T convertRawValueToFinal(const std::string& rawValue) const {
			if constexpr (std::is_same_v<T, std::string>) return rawValue;
			else if constexpr (std::is_same_v<T, int>) return std::stoi(rawValue);
			else if constexpr (std::is_same_v<T, long>) return std::stol(rawValue);
			else if constexpr (std::is_same_v<T, long long>) return std::stoll(rawValue);
			else if constexpr (std::is_same_v<T, unsigned long>) return std::stoul(rawValue);
			else if constexpr (std::is_same_v<T, unsigned long long>) return std::stoull(rawValue);
			else if constexpr (std::is_same_v<T, float>) return std::stof(rawValue);
			else if constexpr (std::is_same_v<T, double>) return std::stod(rawValue);
			else if constexpr (std::is_same_v<T, long double>) return std::stold(rawValue);
			else static_assert(sizeof(T) == 0, "unsupported type T");
		}
		template <typename T> 
		std::string getAvailableParams(const std::map<std::string, T>& section) const {
			std::string out;
			for (const auto& varAndVal : section) {
				if (!out.empty()) out += ", ";
				out += varAndVal.first;
			}
			return out.empty() ?
				"-- none --" :
				out;
		}
		void loadDataFromFile();
		std::pair<std::string, std::string> convertInputStrToKey(std::string_view inputStr) const;
		void removeEdgeSpaces(std::string& str) const;
		void parseSectionLine(std::string_view line, std::size_t numberOfLine, std::string& currentSection);
		void parseVariableLine(std::string_view line, std::size_t numberOfLine, const std::string& currentSection);
		std::map <std::string, // section
			std::map <std::string, std::string>> filedata; // varName - varValue
		std::string filename;
	};
}



