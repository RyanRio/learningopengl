#include "GlGraphicsProgram.h"
#include "ShaderProgramBuilder.h"
#include "iostream"
#include "memory"
#include <string>
#include <math.h>

void GlGraphicsProgram::DebugCallback(GLenum source, GLenum type, GLuint id,
                                      GLenum severity, GLint length,
                                      const GLchar *message,
                                      const GLvoid *userParam​) {
    GlGraphicsProgram::DebugOutputToFile(source, type, id, severity, message);
}

void GlGraphicsProgram::DebugOutputToFile(GLenum source, GLenum type, GLuint id,
                                          GLenum severity,
                                          const GLchar *message) {
    FILE *f;
    f = fopen("Debug.txt", "a");
    if (f){
        char debSource[16], debType[20], debSev[7];
        if (source == GL_DEBUG_SOURCE_API_ARB)
            strcpy(debSource, "OpenGL");
        else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
            strcpy(debSource, "Windows");
        else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
            strcpy(debSource, "Shader Compiler");
        else if (source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
            strcpy(debSource, "Third Party");
        else if (source == GL_DEBUG_SOURCE_APPLICATION_ARB)
            strcpy(debSource, "Application");
        else if (source == GL_DEBUG_SOURCE_OTHER_ARB)
            strcpy(debSource, "Other");
        if (type == GL_DEBUG_TYPE_ERROR_ARB)
            strcpy(debType, "Error");
        else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
            strcpy(debType, "Deprecated behavior");
        else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
            strcpy(debType, "Undefined behavior");
        else if (type == GL_DEBUG_TYPE_PORTABILITY_ARB)
            strcpy(debType, "Portability");
        else if (type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
            strcpy(debType, "Performance");
        else if (type == GL_DEBUG_TYPE_OTHER_ARB)
            strcpy(debType, "Other");
        if (severity == GL_DEBUG_SEVERITY_HIGH_ARB)
            strcpy(debSev, "High");
        else if (severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)
            strcpy(debSev, "Medium");
        else if (severity == GL_DEBUG_SEVERITY_LOW_ARB)
            strcpy(debSev, "Low");
        fprintf(f, "Source:%s\tType:%s\tID:%d\tSeverity:%s\tMessage:%s\n", debSource, debType, id, debSev, message);
        fclose(f);
    }
}


int GlGraphicsProgram::init() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

    // glfw window creation
    // --------------------
    m_window = glfwCreateWindow(m_width, m_height, "LearnOpenGL", NULL, NULL);
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
    glDebugMessageCallbackARB(&GlGraphicsProgram::DebugCallback, NULL);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

    glGenBuffers(2, m_VBOs);
    glGenBuffers(1, &m_EBO);
    glGenVertexArrays(2, m_VAOs);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_ERROR_ARB, 1U, GL_DEBUG_SEVERITY_HIGH_ARB, 17, "The end is near!");
    
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

void GlGraphicsProgram::useShaderProgram(ShaderProgram *prog) {
    m_shaderProgram = prog;
    m_shaderProgram->use();
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
        const Color& c = triangle.cs[v];
        int r = v * 6;
        triangleVertices[r] = p.x;
        triangleVertices[r + 1] = p.y;
        triangleVertices[r + 2] = 0.0f;
        triangleVertices[r + 3] = c.r;
        triangleVertices[r + 4] = c.g;
        triangleVertices[r + 5] = c.b;
    }
    
    // ..:: triangle Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(m_VAOs[0]);
    // 2. copy our triangleVertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    // 3. then set the vertex attributes pointers
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

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
        const Color& c = rect.cs[v];
        const Point &tp = rect.ts[v];
        int r = v * 8;
        rectVertices[r] = p.x;
        rectVertices[r + 1] = p.y;
        rectVertices[r + 2] = 0.0f;

        rectVertices[r + 3] = c.r;
        rectVertices[r + 4] = c.g;
        rectVertices[r + 5] = c.b;

        rectVertices[r + 6] = tp.x;
        rectVertices[r + 7] = tp.y;
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
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GlGraphicsProgram::pollEvents() {
    // glfw: poll IO events (keys pressed/released, mouse moved etc.)
    glfwPollEvents();
}

void GlGraphicsProgram::processInput() {
    // query GLFW whether relevant keys are pressed/released this frame and react accordingly
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
}

void GlGraphicsProgram::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}