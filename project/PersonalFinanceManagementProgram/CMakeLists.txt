cmake_minimum_required(VERSION 3.15)

project(PersonalFinanceManagementProgram)

set(CMAKE_CXX_STANDARD 17)

add_executable(PersonalFinanceManagementProgram 
    main.c 
    date.c
    record.c
    stats.c
    record_list.c
    chart.c
)