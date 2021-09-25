#include "GlGraphicsProgram.h"
#include "ShaderProgramBuilder.h"
#include "iostream"
#include "memory"
#include <string>

int GlGraphicsProgram::init() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, GlGraphicsProgram::framebufferSizeCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glGenBuffers(2, m_VBOs);
    glGenBuffers(1, &m_EBO);
    glGenVertexArrays(2, m_VAOs);

    // compile shaders
    ShaderProgramBuilder builder;
    builder.addFragmentShader("C:\\Users\\therm\\Documents\\learnopengl\\shaders\\fragmentShader.frag");
    builder.addVertexShader("C:\\Users\\therm\\Documents\\learnopengl\\shaders\\vertexShader.vert");
    m_shaderProgram = builder.buildShaderProgram();
    glUseProgram(m_shaderProgram);
    
    return 0;
}

int GlGraphicsProgram::destroy() {
    glDeleteVertexArrays(2, m_VAOs);
    glDeleteBuffers(2, m_VBOs);
    // glDeleteProgram(m_shaderProgram);
    glfwTerminate(); // clear all previously allocated glfw resources
    return 0;
}

bool GlGraphicsProgram::closed() {
    return glfwWindowShouldClose(m_window);
}

void GlGraphicsProgram::setBackground(float red, float green, float blue, float alpha) {
    glClearColor(red, green, blue, alpha);
}

void GlGraphicsProgram::render(RenderChoice choice) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 4 draw the object
    // glUseProgram(m_shaderProgram);
    if (choice == RenderChoice::TRIANGLE) {
        glBindVertexArray(m_VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    } else if (choice == RenderChoice::RECTANGLE) {
        glBindVertexArray(m_VAOs[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    // not technically necessary as you have to bind the vertex array to use anyway
    glBindVertexArray(0);

    // glfw: swap buffers
    glfwSwapBuffers(m_window);
}

void GlGraphicsProgram::setTriangle(const Triangle &triangle) {
    int t = 0;
    for (int v = 0; v < 3; v++) {
        const Point& p = triangle.ps[v];
        int r = v * 3;
        triangleVertices[r] = p.x;
        triangleVertices[r + 1] = p.y;
        triangleVertices[r + 2] = 0.0f;
    }
    
    // ..:: triangle Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(m_VAOs[0]);
    // 2. copy our triangleVertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    // 3. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind the vbo from array buffer, glVertexAttribPointer registered VBO as the vertex attribute's bound VBO so can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GlGraphicsProgram::setRectangle(const Rectangle &rect) {
    // set index data
    memcpy(indices, rect.indices, 6 * sizeof(unsigned int));

    int t = 0;
    for (int v = 0; v < 4; v++) {
        const Point& p = rect.ps[v];
        int r = v * 3;
        rectVertices[r] = p.x;
        rectVertices[r + 1] = p.y;
        rectVertices[r + 2] = 0.0f;
    }
    
    // ..:: Initialization code :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(m_VAOs[1]);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[1]); // rectangle vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GlGraphicsProgram::pollEvents() {
    // glfw: poll IO events (keys pressed/released, mouse moved etc.)
    glfwPollEvents();
}

void GlGraphicsProgram::processInput()
{
    // query GLFW whether relevant keys are pressed/released this frame and react accordingly
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
}

void GlGraphicsProgram::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}