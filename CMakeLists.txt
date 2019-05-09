cmake_minimum_required(VERSION 3.13)
project(upload_SimSearcher)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(upload_SimSearcher
        main.cpp
        SimSearcher.cpp
        SimSearcher.h Trie.cpp Trie.h)
