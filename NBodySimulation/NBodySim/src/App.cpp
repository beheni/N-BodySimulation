#include "App.h"
#include <random>

App::App()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = std::make_unique<Window>(1920, 1080, "N-Body Simulation", true);
    m_RenderProgram = std::make_unique<RenderProgram>("./NBodySim/data/shaders/shader.vert", "./NBodySim/data/shaders/shader.frag");
    m_ComputeProgram = std::make_unique<ComputeProgram>("./NBodySim/data/shaders/shader.comp");
    m_Texture = std::make_unique<Texture>("./NBodySim/data/textures/star.png");
    m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 10.0f, 40.0f), 75.0f, m_Window->GetAspectRation(), 0.1f, 250.0f);
    m_Mesh = std::make_unique<Mesh>(78, -80, 80);
    m_Mouse = std::make_unique<Mouse>(m_Window->Get());


    std::normal_distribution<float> distX(0, 30);
    std::normal_distribution<float> distY(0, 2);
    std::normal_distribution<float> distZ(0, 30);
    std::default_random_engine eng;
    std::vector<glm::vec4> data;
    data.reserve(512 * 512);
    for (size_t i = 0; i < 512 * 512; i++)
    {
        data.emplace_back(distX(eng), distY(eng), distZ(eng), 0.0f);
    }
    m_TexturePos = std::make_unique<Texture>(512, 512, data.data());

    m_Mouse->DisableCursor(m_Window->Get());
}

App::~App()
{
    glfwTerminate();
}

void App::Run()
{
    m_Clock.Restart();
    m_RenderProgram->Use();

    //m_Texture->Bind();

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

    // compute shit
    //m_ComputeProgram->Use();
    //glDispatchCompute(512 / 8, 512 / 4, 1);
    //glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // render shit
    m_RenderProgram->Use();
    m_Texture->Bind(1);
    m_TexturePos->Bind(0);
    //m_TexturePos->BindCompute(0);
    m_RenderProgram->SetInt("u_Texture", 1);
    m_RenderProgram->SetInt("u_TexturePos", 0);
    m_RenderProgram->SetMat4x4("u_ProjView", m_Camera->GetProjectionMatrix() * m_Camera->GetViewMatrix());
    //m_RenderProgram->SetMat4x4("u_Model", glm::rotate(glm::identity<glm::mat4x4>(), (float)glfwGetTime(), glm::vec3(0, 1, 0)));
    m_RenderProgram->SetMat4x4("u_Model", glm::identity<glm::mat4x4>());
    m_RenderProgram->SetMat4x4("u_CameraRotation", m_Camera->GetRotationMatrix());

    m_Window->Clear(0.05f, 0.05f, 0.1f);
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
    glm::vec2 mouseOffset = m_Mouse->GetOffset(m_Window->Get());
    const float sensitivity = 0.03f;
    mouseOffset *= sensitivity;
    m_Camera->ProcessMouseInput(mouseOffset);
}

void App::PollEvents(float dt)
{
    glfwPollEvents();
}
