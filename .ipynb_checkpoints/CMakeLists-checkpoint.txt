cmake_minimum_required(VERSION 3.10)
project(SuffixTreeSearch)

set(CMAKE_CXX_STANDARD 17)

include_directories(code)

add_executable(SuffixTreeSearch
    app/main.cpp
    code/SuffixTree.cpp
)
