cmake_minimum_required(VERSION 3.15)

project(PersonalFinanceManagementProgramV2)

set(CMAKE_CXX_STANDARD 17)

add_executable(PersonalFinanceManagementProgramV2 
    main.c
    model.c 
    storage.c
    stats.c
    charts.c
    import_export.c
    input.c
    menu.c
)