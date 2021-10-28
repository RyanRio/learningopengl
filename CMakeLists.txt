cmake_minimum_required(VERSION 3.11-3.18)

set(CMAKE_VERBOSE_MAKEFILE true)

project(
    LearnOpenGL
    VERSION 0.1
    DESCRIPTION "learning opengl and other tooling")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(src)   # look in src subdirectory

get_target_property(main_basename main OUTPUT_NAME)
get_target_property(main_suffix main SUFFIX)
set(main_filename ${main_basename}${main_suffix})
# make copied file be dependent from one which is build.
# Note, that DEPENDS here creates dependencies both from the target
# and from the file it creates.
add_custom_command(OUTPUT
        copy_all
    COMMAND ${CMAKE_COMMAND} -E copy 
            ${CMAKE_SOURCE_DIR}/external/lib/ktx.dll
            ${CMAKE_CURRENT_SOURCE_DIR}/bin/ktx.dll
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/Assets
            ${CMAKE_CURRENT_SOURCE_DIR}/bin/Assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/shaders
            ${CMAKE_CURRENT_SOURCE_DIR}/bin/shaders
    DEPENDS main
)
# Create target which consume the command via DEPENDS.
add_custom_target(copy_files ALL
    DEPENDS copy_all
)
