// graphics imports
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <KTX/ktx.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// std imports
#include <iostream>
// project imports
#include "GlGraphicsProgram.h"
#include "TextureLoader.h"
#include "Texture.h"
#include "lib/math.h"


#pragma warning(disable : 4996)

using namespace math;

// TODO: better error and logging utilities
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

    ktxTexture2* awesomeFace = TextureLoader<ktxTexture2>::load("Assets/awesomeface.ktx2");
    ktxTexture2* container = TextureLoader<ktxTexture2>::load("Assets/container.ktx2");
    
    Texture2D awesomeFaceTex;
    Texture2D containerTex;

    // set the texture wrapping/filtering options (on the currently bound
    // texture object)
    containerTex.setTexParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    containerTex.setTexParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    containerTex.setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    containerTex.setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    containerTex.glUpload(container);

    // set the texture wrapping/filtering options (on the currently bound
    // texture object)
    awesomeFaceTex.setTexParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    awesomeFaceTex.setTexParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    awesomeFaceTex.setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    awesomeFaceTex.setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    awesomeFaceTex.glUpload(awesomeFace);

    containerTex.setActive(0);
    awesomeFaceTex.setActive(1);

    containerTex.bindShaderProgramSampler(*shaderProg, "texture1");
    awesomeFaceTex.bindShaderProgramSampler(*shaderProg, "texture2");

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

    math::Mat4Handle* handle = allocHandle();
    math::Mat4Handle* handle1 = allocHandle();
    math::Mat4Handle* handle2 = allocHandle();

    glm::mat4 identity = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    while (!prog.closed()) {
        Transformation timedRotation{ (float)glfwGetTime(), glm::vec3(0.5, 1.0, 1.0f), glm::vec3(1.5f), glm::vec3(0.2f, 0.0f, 0.0f) };
        applyTransformation(identity, timedRotation, *handle);
        shaderProg->setMat4("transform", glm::value_ptr(handle->matrix));
        prog.processInput();

        prog.render(RenderChoice::RECTANGLE);

        prog.pollEvents();
    }  

    return prog.destroy();
}
