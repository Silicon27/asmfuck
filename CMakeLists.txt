cmake_minimum_required(VERSION 3.30)
project(turingcomplete LANGUAGES CXX ASM)

set(CMAKE_CXX_STANDARD 20)

add_executable(turingcomplete main.cpp
        src/parser.cpp
        src/ast.cpp
        src/error.cpp
        src/semantic_analysis.cpp
)
