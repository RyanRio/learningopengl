#ifndef GL_GRAPHICS_PROGRAM_H
#define GL_GRAPHICS_PROGRAM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

class Triangle {
public:
    Triangle(const Point& p1, const Point& p2, const Point& p3): ps{p1, p2, p3} {}
    const Point ps[3];

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
    Rectangle(const Point& tr, const Point& br, const Point& bl, const Point& tl): ps{tr, br, bl, tl} {}
    const Point ps[4];
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

    // triangleVertices to send to vertex buffer
    float triangleVertices[9];

    // rectangle vertices to send to vertex buffer
    float rectVertices[12];
    // indices of unique vertices of the rectangle.. 1 index per vertex "drawn"
    unsigned int indices[6];

    GLuint m_VBOs[2];

    GLuint m_VAOs[2];

    GLuint m_EBO;

    GLuint m_shaderProgram; // the shader program in use

};

#endif