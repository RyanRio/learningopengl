#ifndef TEXTURE_H
#define TEXTURE_H
/*  Defines the API for generating and interacting with OpenGL textures  */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <KTX/ktx.h>
#include "ShaderProgramBuilder.h"

class Texture2D {
public:
    Texture2D();

    // uploads data in any format to the GPU
    template <class T>
    void glUpload(T* data){
        return;
    };

    // set specific texture parameters more easily on GL_TEXTURE_2D
    void setTexParameter(GLenum pname, GLint param);

    // set which texture unit to use - GL_TEXTURE0, ... - provide the uint offset from 0
    void setActive(GLuint textureUnit);

    // bind the current texture to the given sampler in a shader program
    void bindShaderProgramSampler(ShaderProgram &program, const char* samplerName);

private:
    unsigned int m_textureID; // the texture id given by genTextures
    unsigned int m_whereActive; // what GL_TEXTURE# this texture is bound to

    void bind();
};

// ktxTexture2 specialization of glUpload
template <>
void Texture2D::glUpload<ktxTexture2>(ktxTexture2* data);

#endif
