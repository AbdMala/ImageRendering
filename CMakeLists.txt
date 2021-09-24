cmake_minimum_required(VERSION 3.16)
project(projectc C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(projectc
        include/common.h
        include/render.h
        include/test_common.h
        include/util.h
        src/common.c
        src/main.c
        src/render.c
        src/test_main.c
        src/unit_tests.c)
