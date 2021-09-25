cmake_minimum_required(VERSION 3.11-3.18)

set(CMAKE_VERBOSE_MAKEFILE true)

project(
    LearnOpenGL
    VERSION 0.1
    DESCRIPTION "learning opengl and other tooling")

# turns off building of glfw docs, tests, examples
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories("external/glfw/include" "external")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(external/glfw)
add_subdirectory(src)   # look in src subdirectory