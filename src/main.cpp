#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GlGraphicsProgram.h"
#include "KTXTexture.h"
#include <iostream>
#include <KTX/ktx.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma warning(disable : 4996)

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
    ShaderProgram* shaderProg = builder.buildShaderProgram();
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

    KTXTexture faceTexture("Assets/awesomeface.ktx2");
    KTXTexture containerTexture("Assets/container.ktx2");
    KTX_error_code result;
    GLenum target;
    GLenum glerror = GL_NO_ERROR;

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound
    // texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    result = ktxTexture_GLUpload((ktxTexture*)containerTexture.m_texture, &texture1, &target, &glerror);
    ktxTexture_Destroy((ktxTexture*)containerTexture.m_texture);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound
    // texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    result = ktxTexture_GLUpload((ktxTexture*)faceTexture.m_texture, &texture2, &target, &glerror);
    ktxTexture_Destroy((ktxTexture*)faceTexture.m_texture);

    GlGraphicsProgram::availableCompressedFormats();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shaderProg->setInt("texture2", 1);
    int bytes;
    int compressed;
    int format;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &compressed);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);
    if (compressed == 1) {
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &bytes);
        std::cout << "compressed image size: " << bytes << std::endl;
        std::cout << "compressed image format: " << format << std::endl;
    }
    else {
        std::cout << "uncompressed image format: " << format << std::endl;
    }
    
    

    while (!prog.closed()) {
        prog.processInput();

        prog.render(RenderChoice::RECTANGLE);

        prog.pollEvents();
    }  

    return prog.destroy();
}
