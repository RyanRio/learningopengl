cmake_minimum_required(VERSION 3.11-3.18)

set(CMAKE_VERBOSE_MAKEFILE true)

project(
    LearnOpenGL
    VERSION 0.1
    DESCRIPTION "learning opengl and other tooling")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)   # look in src subdirectory