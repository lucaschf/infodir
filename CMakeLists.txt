cmake_minimum_required(VERSION 3.17)
project(infodir C)


set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -pthread)

add_executable(infodir infodir.c infodir.h)
