cmake_minimum_required(VERSION 3.26)
project(mbr C)

set(CMAKE_C_STANDARD 11)

configure_file(mbr.bin mbr.bin COPYONLY)
add_executable(mbr main.c)
