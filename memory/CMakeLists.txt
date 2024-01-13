cmake_minimum_required(VERSION 3.26)
project(memory C)

set(CMAKE_C_STANDARD 17)

add_executable(memory main.c
        memoria.h
        memoria.c
        lista.h
        lista.c)
