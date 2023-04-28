#include "Window.h"
#include "Exception.h"

Window::Window(int width, int height, const char* title, bool fullscreen)
    :
    m_Title(title), m_FullScreen(fullscreen)
{
    if (m_FullScreen)
    {
        auto monitor = glfwGetPrimaryMonitor();
        auto mode = glfwGetVideoMode(monitor);
        m_Width = mode->width;
        m_Height = mode->height;
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), glfwGetPrimaryMonitor(), NULL);
    }
    else
    {
        m_Width = width;
        m_Height = height;
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    }

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

    glViewport(0, 0, m_Width, m_Height);
    glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
}

GLFWwindow* Window::Get() const
{
	return m_Window;
}

bool Window::Open() const
{
    return !glfwWindowShouldClose(m_Window);
}

float Window::GetAspectRation() const
{
    return static_cast<float>(m_Width) / static_cast<float>(m_Height);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

void Window::Clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Close()
{
    glfwSetWindowShouldClose(m_Window, true);
}

void Window::SetFullScreen(bool flag)
{
    if (m_FullScreen == flag) return;

    m_FullScreen = flag;
    glfwDestroyWindow(m_Window);

    if (m_FullScreen)
    {
        auto monitor = glfwGetPrimaryMonitor();
        auto mode = glfwGetVideoMode(monitor);
        m_Width = mode->width;
        m_Height = mode->height;
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), glfwGetPrimaryMonitor(), NULL);
    }
    else
    {
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    }

    if (m_Window == NULL)
    {
        throw EXCEPTION("Failed to create window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, Window::FramebufferSizeCallBack);
    glViewport(0, 0, m_Width, m_Height);
}

void Window::FramebufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
