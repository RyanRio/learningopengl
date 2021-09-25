#include "ShaderProgramBuilder.h"
#include "iostream"
#include "fstream"

void ShaderProgramBuilder::addFragmentShader(std::string fileName) {
    int success;
    char infoLog[512];

    std::string fragmentShaderSource = readShaderFromFile(fileName);
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSource = fragmentShaderSource.c_str();
    glShaderSource(m_fragmentShader, 1, &fSource, NULL);
    glCompileShader(m_fragmentShader);
    // check for frag shader compile errors
    glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success); // query shader for compile status
    if (!success) {
        glGetShaderInfoLog(m_fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
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
    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success); // query shader for compile status
    if (!success) {
        glGetShaderInfoLog(m_vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

int ShaderProgramBuilder::buildShaderProgram() {
    GLuint m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glLinkProgram(m_shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        return success;
    }

    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
    return m_shaderProgram;
}

std::string ShaderProgramBuilder::readShaderFromFile(std::string fileName) {
    std::string content;
    std::ifstream fileStream(fileName, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << fileName << ". File does not exist." << std::endl;
        fileStream.close();
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}
