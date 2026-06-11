cmake_minimum_required(VERSION 3.15)

project(WordCountingTool)

set(CMAKE_CXX_STANDARD 17)

add_executable(WordCountingTool 
    main.c 
    file_reader.c
    word_stat.c
)