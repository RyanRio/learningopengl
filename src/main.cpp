#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GlGraphicsProgram.h"
#include "KTXTexture.h"
#include <iostream>
#include <KTX/ktx.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool hasExtension(std::string extension) {
    int num_extensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
    for (int i = 0; i < num_extensions; i++) {
        std::string cur(reinterpret_cast<const char *>(
            glGetStringi(GL_EXTENSIONS, i)));
        if (extension == cur) {
            return true;
        }
    }
    std::cout << "ERROR::EXTENSION"
              << "\n " << extension << " is not supported." << std::endl;
    return false;
}

int main()
{
        
    GlGraphicsProgram prog(800, 600);
    prog.init();
    if (!hasExtension("GL_ARB_texture_compression_bptc")) {
        exit(1);
    }
    // bringing shader program out here... kind of awkward since we specifically have to initialize it now up here
    // compile shaders
    // float timeValue = glfwGetTime();
    // float greenValue = sin(timeValue) / 2.0f + 0.5f;
    // int vertexColorLocation = glGetUniformLocation(m_shaderProgram, "ourColor");
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); example of setting uniform
    ShaderProgramBuilder builder;
    builder.addFragmentShader("shaders\\colorFromVS.frag");
    builder.addVertexShader("shaders\\vertexShader.vert");
    ShaderProgram *shaderProg = builder.buildShaderProgram();
    prog.useShaderProgram(shaderProg);
    // shaderProg->setFloat("alpha", 0.5f); // Need to use shader program before setting uniforms

    prog.setBackground(0.2f, 0.3f, 0.3f, 1.0f);

    Triangle mainTriangle({0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {-0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.5f}, {0.0f, 0.0f, 1.0f});

    // create tr, br, bl, tl points now
    Rectangle mainRectangle({0.5f, 0.5f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // top right
                            {0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, // bottom right
                            {-0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, // bottom left
                            {-0.5f, 0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}); // top left

    prog.setTriangle(mainTriangle);
    prog.setRectangle(mainRectangle);

    int num_formats;
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num_formats);


    KTXTexture textureContainer("Assets/awesomeface.ktx2");
    KTX_error_code result;
    GLenum target;
    GLenum glerror = GL_NO_ERROR;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound
    // texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    result = ktxTexture_GLUpload((ktxTexture*)textureContainer.m_texture, &texture, &target, &glerror);
    ktxTexture_Destroy((ktxTexture *)textureContainer.m_texture);

    while (!prog.closed()) {
        prog.processInput();

        prog.render(RenderChoice::RECTANGLE);

        prog.pollEvents();
    }  

    return prog.destroy();
}