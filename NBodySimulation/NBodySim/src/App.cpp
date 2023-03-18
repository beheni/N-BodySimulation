#include "App.h"

App::App()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = std::make_unique<Window>(1920, 1080, "N-Body Simulation", true);
    m_RenderShader = std::make_unique<RenderShader>("./NBodySim/data/shaders/shader.vert", "./NBodySim/data/shaders/shader.frag");
    m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 2.0f), 75.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    m_Mesh = std::make_unique<Mesh>(100);
}

App::~App()
{
    glfwTerminate();
}

void App::Run()
{
    m_Clock.Restart();

    while (m_Window->Open())
    {
        float deltaTime = m_Clock.Stamp();
        // Processing user input
        ProcessEvents(deltaTime);

        // Rendering stuff goes here
        DoFrame(deltaTime);

        PollEvents(deltaTime);
        m_Window->SwapBuffers();
    }
}

void App::DoFrame(float dt)
{
    m_RenderShader->Use();

    m_RenderShader->SetFloat("ColorScale", sin(glfwGetTime()) / 2.0f + 0.5f);
    m_RenderShader->SetMat4x4("Projection", m_Camera->GetProjectionMatrix());
    m_RenderShader->SetMat4x4("View", m_Camera->GetViewMatrix());
    m_RenderShader->SetMat4x4("Model", glm::identity<glm::mat4x4>());

    m_Window->Clear(0.7f, 0.2f, 0.4f);
    m_Mesh->Draw();
}

void App::ProcessEvents(float dt)
{
    if (glfwGetKey(m_Window->Get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        m_Window->Close();

    // Camera movement
    using CamDir = Camera::Direction;
    if (glfwGetKey(m_Window->Get(), GLFW_KEY_A) == GLFW_PRESS)
        m_Camera->Move(CamDir::Left, dt);
    if (glfwGetKey(m_Window->Get(), GLFW_KEY_D) == GLFW_PRESS)
        m_Camera->Move(CamDir::Right, dt);
    if (glfwGetKey(m_Window->Get(), GLFW_KEY_W) == GLFW_PRESS)
        m_Camera->Move(CamDir::Front, dt);
    if (glfwGetKey(m_Window->Get(), GLFW_KEY_S) == GLFW_PRESS)
        m_Camera->Move(CamDir::Back, dt);
    if (glfwGetKey(m_Window->Get(), GLFW_KEY_SPACE) == GLFW_PRESS)
        m_Camera->Move(CamDir::Up, dt);
    if (glfwGetKey(m_Window->Get(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_Camera->Move(CamDir::Down, dt);

    // Camera rotation
}

void App::PollEvents(float dt)
{
    glfwPollEvents();
}
