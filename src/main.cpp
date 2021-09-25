#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "iostream"
#include "memory"
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

class Rectangle {
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
    // compile shaders and return status
    int compileShaders();
    // create and link the shader program, using compiled shaders
    int linkShaderProgram(GLuint *vs, GLuint *fs);

    // triangleVertices to send to vertex buffer
    float triangleVertices[9];

    // rectangle vertices to send to vertex buffer
    float rectVertices[12];
    // indices of unique vertices of the rectangle.. 1 index per vertex "drawn"
    unsigned int indices[6];

    GLuint m_VBOs[2];

    GLuint m_VAOs[2];

    GLuint m_EBO;

    GLuint m_vertexShader;
    GLuint m_fragmentShader;
    GLuint m_shaderProgram;

    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
};

int GlGraphicsProgram::init() {
    int status = 0;
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
    if (!compileShaders()) {
        return -1;
    }
    if (!linkShaderProgram(&m_vertexShader, &m_fragmentShader)) {
        return -1;
    }
    
    return 0;
}

int GlGraphicsProgram::compileShaders() {
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(m_vertexShader);

    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(m_fragmentShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success); // query shader for compile status
    if (!success) {
        glGetShaderInfoLog(m_vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return success;
    }
    glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success); // query shader for compile status
    if (!success) {
        glGetShaderInfoLog(m_fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return success;
    }
    return 0;
}

int GlGraphicsProgram::linkShaderProgram(GLuint *vs, GLuint *fs) {
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, *vs);
    glAttachShader(m_shaderProgram, *fs);
    glLinkProgram(m_shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        return success;
    }

    glUseProgram(m_shaderProgram);
    glDeleteShader(*vs);
    glDeleteShader(*fs);
    *vs = 0;
    *fs = 0;
    return 0;
}

int GlGraphicsProgram::destroy() {
    glDeleteVertexArrays(2, m_VAOs);
    glDeleteBuffers(2, m_VBOs);
    glDeleteProgram(m_shaderProgram);
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
    glClear(GL_COLOR_BUFFER_BIT);

    // 4 draw the object
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
}

void GlGraphicsProgram::pollEvents() {
    // glfw: poll IO events (keys pressed/released, mouse moved etc.)
    glfwPollEvents();
}

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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!prog.closed()) {
        prog.processInput();

        prog.render(RenderChoice::TRIANGLE);

        prog.pollEvents();
    }
    
    return prog.destroy();
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