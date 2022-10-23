cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(NPO_Colibri LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(NPO_Colibri main.cpp encoder.h encoder.cpp funcs.cpp funcs.h)
