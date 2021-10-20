#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GlGraphicsProgram.h"
#include <iostream>
#include <KTX/ktx.h>

int main()
{
    ktxTexture *texture;
    KTX_error_code result;
    ktx_size_t offset;
    ktx_uint8_t *image;
    ktx_uint32_t level, layer, faceSlice;

    result = ktxTexture_CreateFromNamedFile(
        "Assets/mytex3d.ktx", KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &texture);

    // Retrieve information about the texture from fields in the ktxTexture
    // such as:
    ktx_uint32_t numLevels = texture->numLevels;
    ktx_uint32_t baseWidth = texture->baseWidth;
    ktx_bool_t isArray = texture->isArray;

    // Retrieve a pointer to the image for a specific mip level, array layer
    // & face or depth slice.
    level = 1;
    layer = 0;
    faceSlice = 3;
    result =
        ktxTexture_GetImageOffset(texture, level, layer, faceSlice, &offset);
    image = ktxTexture_GetData(texture) + offset;
    // ...
    // Do something with the texture image.
    // ...
    ktxTexture_Destroy(texture);
    
    GlGraphicsProgram prog(600, 800);
    prog.init();
    // bringing shader program out here... kind of awkward since we specifically have to initialize it now up here
    // compile shaders
    // float timeValue = glfwGetTime();
    // float greenValue = sin(timeValue) / 2.0f + 0.5f;
    // int vertexColorLocation = glGetUniformLocation(m_shaderProgram, "ourColor");
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); example of setting uniform
    ShaderProgramBuilder builder;
    builder.addFragmentShader("C:\\Users\\therm\\source\\repos\\learnopengl\\shaders\\colorFromVS.frag");
    builder.addVertexShader("C:\\Users\\therm\\source\\repos\\learnopengl\\shaders\\vertexShader.vert");
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