#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GlGraphicsProgram.h"
#include <iostream>

int main()
{
    GlGraphicsProgram prog(600, 800);
    prog.init();
    prog.setBackground(0.2f, 0.3f, 0.3f, 1.0f);

    Triangle mainTriangle({-0.5f, -0.5f}, {0.5f, -0.5f}, {0.0f, 0.5f});

    // create tr, br, bl, tl points now
    Rectangle mainRectangle({0.5f, 0.5f}, {0.5f, -0.5f}, {-0.5f, -0.5f}, {-0.5f, 0.5f});

    prog.setTriangle(mainTriangle);
    prog.setRectangle(mainRectangle);

    while (!prog.closed()) {
        prog.processInput();

        prog.render(RenderChoice::RECTANGLE);

        prog.pollEvents();
    }
    
    return prog.destroy();
}