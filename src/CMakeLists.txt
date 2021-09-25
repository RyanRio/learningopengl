cmake_minimum_required(VERSION 3.11-3.18)

add_executable(main)

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_sources(main PRIVATE
                main.cpp
                GlGraphicsProgram.cpp
                ShaderProgramBuilder.cpp
                glad.c)

target_link_libraries(main glfw)

# Tell C++ compiler to use C++20 features. We don't actually use any of them.
target_compile_features(main PUBLIC cxx_std_20)