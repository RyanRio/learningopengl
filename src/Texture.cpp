#include "Texture.h"

Texture2D::Texture2D() : m_textureID(0), m_whereActive(0) {
    glGenTextures(1, &m_textureID);
}

void Texture2D::bind() {
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture2D::setTexParameter(GLenum pname, GLint param) {
    bind();
    glTexParameteri(GL_TEXTURE_2D, pname, param);
}

void Texture2D::setActive(GLuint textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    m_whereActive = textureUnit;
    bind();
}

void Texture2D::bindShaderProgramSampler(ShaderProgram& program, const char* samplerName) {
    program.setInt(samplerName, m_whereActive);
}

template <>
void Texture2D::glUpload<ktxTexture2>(ktxTexture2* data) {
    bind();
    GLenum target, glerror;
    KTX_error_code result = ktxTexture_GLUpload((ktxTexture*)data, &m_textureID, &target, &glerror);
    ktxTexture_Destroy((ktxTexture*)data);
}