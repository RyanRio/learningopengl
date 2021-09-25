#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum ShaderType { VERTEX_SHADER = 0, FRAGMENT_SHADER };

class ShaderProgramBuilder {
public:
    // add a vertex shader from file to the current configuration
    void addVertexShader(std::string fileName);
    // add a fragment shader from file to the current configuration
    void addFragmentShader(std::string fileName);
    // get a shader program from the current configuration
    int buildShaderProgram();
    // create a builder
    ShaderProgramBuilder() = default;
private:
    GLuint m_vertexShader;
    GLuint m_fragmentShader;

    static std::string readShaderFromFile(std::string fileName);
};

#endif