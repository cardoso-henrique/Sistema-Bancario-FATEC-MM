cmake_minimum_required(VERSION 3.26)
project(sistemabancario C)

set(CMAKE_C_STANDARD 17)

add_executable(sistemabancario main.c
        conta.h
        conta.c)
