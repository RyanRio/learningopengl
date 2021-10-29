#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum ShaderType { VERTEX_SHADER = 0, FRAGMENT_SHADER };

// represents a built shader program, can set uniform values
class ShaderProgram {
public:
    ShaderProgram(GLuint programId): id(programId) {}

    // use this shader program in the current gl context
    void use();

    // utility uniform functions
    // set a single bool uniform value
    void setBool(const std::string &name, bool value) const;
    // set a single int uniform value
    void setInt(const std::string &name, int value) const;
    // set a single float uniform value
    void setFloat(const std::string &name, float value) const;
    // set a single mat4 uniform value
    void setMat4(const std::string &name, float* data) const;

private:
    unsigned int id;
    // get the uniform location and print error if bad name
    GLint getUniformLocation(const GLchar *name) const;
};

// builds a shader program
class ShaderProgramBuilder {
public:
    // add a vertex shader from file to the current configuration
    void addVertexShader(std::string fileName);
    // add a fragment shader from file to the current configuration
    void addFragmentShader(std::string fileName);
    // get a shader program from the current configuration
    ShaderProgram* buildShaderProgram();
    // create a builder
    ShaderProgramBuilder() = default;
private:
    GLuint m_vertexShader;
    GLuint m_fragmentShader;

    // utility function for checking shader compilation errors
    void checkCompileErrors(unsigned int shader);
    void checkLinkErrors(unsigned int id);

    static std::string readShaderFromFile(std::string fileName);
};

#endif