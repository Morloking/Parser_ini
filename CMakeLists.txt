cmake_minimum_required(VERSION 3.22)
project(ini_parser CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(INI-Parser
main.cpp
parser.cpp
)

add_custom_command(TARGET INI-Parser POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
${CMAKE_SOURCE_DIR}/input.ini
$<TARGET_FILE_DIR:INI-Parser>/input.ini
)