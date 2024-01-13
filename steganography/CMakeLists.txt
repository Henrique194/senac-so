cmake_minimum_required(VERSION 3.26)
project(steganography C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FUSE REQUIRED fuse3)


add_executable(steganography main.c)

target_compile_options(steganography PRIVATE ${FUSE_CFLAGS})
target_link_libraries(steganography PRIVATE ${FUSE_LIBRARIES})
