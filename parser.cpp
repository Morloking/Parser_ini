#include "parser.h"

#include <fstream>
namespace core {
    void Parser::loadDataFromFile() {
        std::ifstream file(filename);
        if (!file.is_open()) 
            throw std::runtime_error("Can't open file: " + filename);

        std::string line;
        std::string currentSection;
        std::size_t numOfLine{ consts::zero };
        while (std::getline(file, line)) {
            ++numOfLine;
            //clear spaces
            removeEdgeSpaces(line);
            if (line.empty()) continue; 
            //clear comments
            if (line.front() == consts::commMark) continue;
            std::size_t commPos = line.find(consts::commMark);
            if (commPos != std::string::npos) { 
                line.erase(commPos);
                removeEdgeSpaces(line);
                if (line.empty()) continue;
            }
            //general parse
            if (line.front() == consts::sectionOpen) 
                parseSectionLine(line, numOfLine, currentSection);
            else 
                parseVariableLine(line, numOfLine, currentSection);
        }
    }
    std::pair<std::string, std::string> Parser::convertInputStrToKey(std::string_view inputStr) const {
        std::size_t pointPos = inputStr.find(consts::pointMark);
        if (pointPos == std::string_view::npos) 
            throw std::runtime_error(std::string(consts::expectedInputFormat));
        std::string section(inputStr.substr(0, pointPos));
        removeEdgeSpaces(section);
        if (section.empty()) 
            throw std::runtime_error("Incorrect input: section is empty" + std::string(consts::expectedInputFormat));
        std::string varName(inputStr.substr(pointPos + 1));
        removeEdgeSpaces(varName);
        if (varName.empty())
            throw std::runtime_error("Incorrect input: varName is empty" + std::string(consts::expectedInputFormat));
        return std::pair<std::string, std::string>(
            std::move(section),
            std::move(varName)
        );
    }
    void Parser::removeEdgeSpaces(std::string& str) const {
        if (str.empty()) return;
        const auto start = str.find_first_not_of(consts::trimmingKit);
        if (start == std::string::npos) {
            str.clear();
            return;
        }
        const auto end = str.find_last_not_of(consts::trimmingKit);
        str = str.substr(start, end - start + 1);
    }
    void Parser::parseSectionLine(std::string_view line, std::size_t numberOfLine, std::string& currentSection) {
        if (line.size() < 2 || line.back() != consts::sectionClose)
            throw std::runtime_error("The section is not closed, line: " + std::to_string(numberOfLine));
        // delete [ and ]
        line.remove_prefix(1);
        line.remove_suffix(1);
        std::string section(line);
        removeEdgeSpaces(section);
        if (section.empty()) throw std::runtime_error("Empty formatted section, line: " + std::to_string(numberOfLine));
        filedata[section];
        currentSection = std::move(section);
    }
    void Parser::parseVariableLine(std::string_view line, std::size_t numberOfLine, const std::string& currentSection) {
        if (currentSection.empty())
            throw std::runtime_error("Variable outside the section, line: " + std::to_string(numberOfLine));
        std::size_t equalPos = line.find(consts::equalMark);
        if (equalPos == std::string_view::npos)
            throw std::runtime_error("Incorrect syntax: '=' is missing, line: " + std::to_string(numberOfLine));
        std::string key(line.substr(0, equalPos));
        std::string value(line.substr(equalPos + 1));
        removeEdgeSpaces(key);
        removeEdgeSpaces(value);
        if (key.empty())
            throw std::runtime_error("Empty key, line: " + std::to_string(numberOfLine));
        filedata[currentSection][std::move(key)] = std::move(value);
    }

}