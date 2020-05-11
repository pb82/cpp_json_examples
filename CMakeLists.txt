cmake_minimum_required(VERSION 3.16)
project(jsonsample)

set(CMAKE_CXX_STANDARD 14)

add_executable(jsonsample main.cpp json/parser.cpp json/printer.cpp io/file.cpp)