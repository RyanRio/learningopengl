cmake_minimum_required(VERSION 3.11-3.18)

add_library(glfw3 STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(glfw3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/lib/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/external/include"
)

add_library(ktx STATIC IMPORTED)
set_target_properties(ktx PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/lib/ktx.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/external/include/KTX")

 add_executable(main)

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_sources(main PRIVATE
                main.cpp
                GlGraphicsProgram.cpp
                ShaderProgramBuilder.cpp
                glad.c)

target_link_libraries(main glfw3 ktx) # also adds the required include path


# Tell C++ compiler to use C++20 features. We don't actually use any of them.
target_compile_features(main PUBLIC cxx_std_20)