#ifndef GL_GRAPHICS_PROGRAM_H
#define GL_GRAPHICS_PROGRAM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgramBuilder.h"
#include <string>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Point {
public:
    Point(float x, float y): x(x), y(y) {}
    float x, y;

    Point(Point const &) = default; // allow copy constructor
    void operator=(Point const &) = delete;
};

class Color {
public:
    Color(float r, float g, float b): r(r), g(g), b(b) {}
    float r, g, b;

    Color(Color const &) = default; // allow copy constructor
    void operator=(Color const &) = delete;
};

class Triangle {
public:
    Triangle(const Point& p1, const Color& c1, const Point& p2, const Color& c2, const Point& p3, const Color& c3): ps{p1, p2, p3}, cs{c1, c2, c3} {}
    const Point ps[3];
    const Color cs[3];

    Triangle(Triangle const &) = delete;
    void operator=(Triangle const &) = delete;
};

/**
 * Example rectangle
 * 3     0
 *  ----- 
 *  |\  |
 *  | \ |
 *  |  \|
 *  -----
 * 2     1
 * 
 * first triangle: tr, br, tl (0, 1, 3)
 * second triangle: br, bl, tl (1, 2, 3)
 */
class Rectangle {
public:
    // top right, bottom right, bottom left, top left
    Rectangle(const Point& tr, const Color& trc, const Point& br, const Color& brc, const Point& bl, const Color& blc, const Point& tl, const Color& tlc): ps{tr, br, bl, tl}, cs{trc, brc, blc, tlc} {}
    const Point ps[4];
    const Color cs[4];
    unsigned int indices[6] = {0, 1, 3, 1, 2, 3};

    Rectangle(Rectangle const &) = delete;
    void operator=(Rectangle const &) = delete;
};

enum RenderChoice {
    TRIANGLE = 0, RECTANGLE
};

class GlGraphicsProgram {
public:
    GlGraphicsProgram(unsigned int width, unsigned int height): m_width(width), m_height(height) {};
    // initialize graphics program
    int init();
    // destroy graphics program
    int destroy();
    // set background
    void setBackground(float red, float green, float blue, float alpha);
    // has this graphics program ended
    bool closed();
    // process all input
    void processInput();
    // poll for new events
    void pollEvents();
    // use a specific shader for this graphics program
    void useShaderProgram(ShaderProgram *shader);
    // render current data to viewport
    void render(RenderChoice choice);
    // set the triangle that this graphics program draws
    void setTriangle(const Triangle &triangle);
    // set the rectangle that this graphics program draws
    void setRectangle(const Rectangle &rect);

private:
    GLuint m_width, m_height;
    GLFWwindow *m_window;
    // glfw callback: whenever the window size changed (by OS or user resize) this callback function executes
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    // triangleVertices to send to vertex buffer (with color data in between each vertex.. v1, c1, v2, c2, ...)
    float triangleVertices[18];

    // rectangle vertices to send to vertex buffer
    float rectVertices[24];
    // indices of unique vertices of the rectangle.. 1 index per vertex "drawn"
    unsigned int indices[6];

    GLuint m_VBOs[2];

    GLuint m_VAOs[2];

    GLuint m_EBO;

    ShaderProgram* m_shaderProgram; // the shader program in use

};

#endif