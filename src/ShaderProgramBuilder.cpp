#include "ShaderProgramBuilder.h"
#include "iostream"
#include "fstream"
#include "sstream"

void ShaderProgram::use() {
    glUseProgram(id);
}

void ShaderProgram::setBool(const std::string &name, bool value) const {         
    glUniform1i(getUniformLocation(name.c_str()), (int)value); 
}

void ShaderProgram::setInt(const std::string &name, int value) const { 
    glUniform1i(getUniformLocation(name.c_str()), value); 
}

void ShaderProgram::setFloat(const std::string &name, float value) const { 
    // int vertexColorLocation = glGetUniformLocation(m_shaderProgram, "ourColor");
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); example of setting uniform
    glUniform1f(getUniformLocation(name.c_str()), value); 
}

GLint ShaderProgram::getUniformLocation(const GLchar *name) const {
    int location = glGetUniformLocation(id, name);
    if (location == -1) {
        std::cout << "ERROR::SHADER::UNIFORM::INVALID" << std::endl;
    }
    return location;
}

void ShaderProgramBuilder::addFragmentShader(std::string fileName) {
    int success;
    char infoLog[512];

    std::string fragmentShaderSource = readShaderFromFile(fileName);
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSource = fragmentShaderSource.c_str();
    glShaderSource(m_fragmentShader, 1, &fSource, NULL);
    glCompileShader(m_fragmentShader);
    // check for frag shader compile errors
    checkCompileErrors(m_fragmentShader);
}

void ShaderProgramBuilder::addVertexShader(std::string fileName) {
    int success;
    char infoLog[512];

    std::string vertexShaderSource = readShaderFromFile(fileName);

    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vSource = vertexShaderSource.c_str();
    glShaderSource(m_vertexShader, 1, &vSource, NULL);
    glCompileShader(m_vertexShader);
    // check for vert shader compile errors
    checkCompileErrors(m_vertexShader);
}

ShaderProgram *ShaderProgramBuilder::buildShaderProgram() {
    GLuint m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glLinkProgram(m_shaderProgram);

    checkLinkErrors(m_shaderProgram);

    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
    return new ShaderProgram(m_shaderProgram);
}

void ShaderProgramBuilder::checkCompileErrors(unsigned int shader) {
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR \n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}

void ShaderProgramBuilder::checkLinkErrors(unsigned int id) {
    int success;
    char infoLog[1024];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}

std::string ShaderProgramBuilder::readShaderFromFile(std::string fileName) {
    std::string code;
    std::ifstream shaderFile;
    // ensure ifstream can throw exceptions
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        // open file
        shaderFile.open(fileName);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handler
        shaderFile.close();
        // convert stream into string
        code = shaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    return code;
}
