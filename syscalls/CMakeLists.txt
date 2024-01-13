cmake_minimum_required(VERSION 3.26)
project(syscalls C)

set(CMAKE_C_STANDARD 11)

add_executable(syscalls main.c
        other.c)
