#version 460 core
#extension GL_OES_compressed_ETC1_RGB8_texture : enable

out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}