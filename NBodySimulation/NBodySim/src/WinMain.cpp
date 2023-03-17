#define NOMINMAX
#include "SafeWindows.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Exception.h"
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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
  
    try
    {
        RenderShader shaderProgram("./NBodySim/data/shaders/shader.vert", "./NBodySim/data/shaders/shader.frag");

        // VAO
        GLuint VAO{};
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Render stuff
        float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0,
         0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0,
         0.0f,  0.5f, 0.0f, 0.0, 0.0, 1.0,
        };
        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        VertexBufferLayout{}
            .Push(3, GL_FLOAT, false) // pos
            .Push(3, GL_FLOAT, false) // col
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
    }
    catch (const Exception& e)
    {
        MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONEXCLAMATION);
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
