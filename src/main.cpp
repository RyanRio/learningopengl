#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GlGraphicsProgram.h"
#include <iostream>

int main()
{
    
    GlGraphicsProgram prog(600, 800);
    prog.init();
    // bringing shader program out here... kind of awkward since we specifically have to initialize it now up here
    // compile shaders
    // float timeValue = glfwGetTime();
    // float greenValue = sin(timeValue) / 2.0f + 0.5f;
    // int vertexColorLocation = glGetUniformLocation(m_shaderProgram, "ourColor");
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); example of setting uniform
    ShaderProgramBuilder builder;
    builder.addFragmentShader("C:\\Users\\therm\\Documents\\learnopengl\\shaders\\colorFromVS.frag");
    builder.addVertexShader("C:\\Users\\therm\\Documents\\learnopengl\\shaders\\vertexShader.vert");
    ShaderProgram *shaderProg = builder.buildShaderProgram();
    prog.useShaderProgram(shaderProg);
    shaderProg->setFloat("alpha", 0.5f); // Need to use shader program before setting uniforms

    prog.setBackground(0.2f, 0.3f, 0.3f, 1.0f);

    Triangle mainTriangle({0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {-0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.5f}, {0.0f, 0.0f, 1.0f});

    // create tr, br, bl, tl points now
    Rectangle mainRectangle({0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {-0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f});

    prog.setTriangle(mainTriangle);
    prog.setRectangle(mainRectangle);

    while (!prog.closed()) {
        prog.processInput();

        prog.render(RenderChoice::RECTANGLE);

        prog.pollEvents();
    }
    
    return prog.destroy();
}