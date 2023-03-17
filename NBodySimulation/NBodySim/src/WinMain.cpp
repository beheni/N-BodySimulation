#define NOMINMAX
#include "SafeWindows.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexBufferLayout.h"
#include "RenderShader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow* window);

int WINAPI wWinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	PWSTR pCmdLine, 
	int nCmdShow)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }
    glViewport(0, 0, 800, 600);
    /*
    // vertex shader
    const char* vertexShaderSource = "#version 430 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aCol;\n"
        "out vec3 Col;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "   Col = aCol;\n"
        "}\0";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        MessageBoxA(nullptr, infoLog, "Vertex shader compile exception", MB_OK | MB_ICONEXCLAMATION);
    }

    // fragment shader
    const char* fragmentShaderSource = "#version 430 core\n"
        "in vec3 Col;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(Col, 1.0);\n"
        "}\0";

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLint _success;
    GLchar _infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &_success);

    if (!_success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, _infoLog);
        MessageBoxA(nullptr, _infoLog, "Fragment shader compile exception", MB_OK | MB_ICONEXCLAMATION);
    }

    // shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint __success;
    GLchar __infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &__success);
    if (!__success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, __infoLog);
        MessageBoxA(nullptr, __infoLog, "Program linking exception", MB_OK | MB_ICONEXCLAMATION);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    */

    RenderShader shaderProgram("./NBodySim/data/shaders/shader.vert", "./NBodySim/data/shaders/shader.frag");

    // VAO
    GLuint VAO{};
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Render stuff
    float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0, 0.0, 1.0,
     0.5f, -0.5f, 0.0f, 0.0, 1.0, 1.0,
     0.0f,  0.5f, 0.0f, 0.0, 0.0, 1.0,
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    VertexBufferLayout{}
        .Push(3, GL_FLOAT, false)
        .Push(3, GL_FLOAT, false)
        .Bind();


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
