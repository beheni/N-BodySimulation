#include "Window.h"
#include "Exception.h"

Window::Window(int width, int height, const char* title)
    :
    m_Width(width), m_Height(height), m_Title(title)
{
    m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (m_Window == NULL)
    {
        throw EXCEPTION("Failed to create window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, Window::FramebufferSizeCallBack);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw EXCEPTION("Failed to init glad.");
    }

    glViewport(0, 0, width, height);
}

Window::~Window()
{
}

GLFWwindow* Window::Get() const
{
	return m_Window;
}

bool Window::Open() const
{
    return !glfwWindowShouldClose(m_Window);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

void Window::Clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Close()
{
    glfwSetWindowShouldClose(m_Window, true);
}

void Window::FramebufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
