#include "App.h"
#include <random>

App::App()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = std::make_unique<Window>(1280, 720, "N-Body Simulation");
    m_RenderProgram = std::make_unique<RenderProgram>("./data/shaders/shader.vert", "./data/shaders/shader.frag");
    m_ComputeProgram = std::make_unique<ComputeProgram>("./data/shaders/shader.comp");
    m_MortonCodesComputeProgram = std::make_unique<ComputeProgram>("./data/shaders/mortonCodes.comp");
    m_Texture = std::make_unique<Texture>("./data/textures/star.png");
    m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 150.0f), 75.0f, m_Window->GetAspectRation(), 0.1f, 1000.0f);
    m_Mesh = std::make_unique<Mesh>(c_TextureSize * c_TextureSize, -80, 80);
    m_Mouse = std::make_unique<Mouse>(m_Window->Get());
    m_Menu = std::make_unique<Menu>("Menu", m_Window->Get());

    Menu::EmbraceTheDarkness();
    m_Menu->AddSliderFloat("Speed", &m_SimulationSpeed, 0.1f, 5.0f);
    m_Menu->AddCheckbox("Simulation", &m_RunSim);
    m_Menu->AddButton("Enter FlyCam Mode", [this] {m_Mouse->DisableCursor(m_Window->Get()); });
    m_Menu->AddText("[press TAB - to exit]");
    //add wasd and arrows

    glm::vec4 data[128*128];
    //m_VelocityTextures.push_back(std::make_unique<Texture>(c_TextureSize, c_TextureSize, data.data()));
    //m_VelocityTextures.push_back(std::make_unique<Texture>(c_TextureSize, c_TextureSize, data.data()));
    
    std::vector<unsigned int> initMortonCodes(c_TextureSize * c_TextureSize);
    m_MortonCodesTexture = std::make_unique<Texture>(c_TextureSize, c_TextureSize, initMortonCodes.data());

    std::normal_distribution<float> distX(0, 10);
    std::normal_distribution<float> distY(0, 10);
    std::normal_distribution<float> distZ(0, 10);
    std::default_random_engine eng;
    //data.reserve(c_TextureSize * c_TextureSize);
    for (size_t i = 0; i < c_TextureSize * c_TextureSize; i++)
    {
        glm::vec3 pos = { distX(eng), distY(eng), distZ(eng) };
        data[i] = glm::vec4(pos.x, pos.y, pos.z, 0.0f);
    }

    GLuint posInputSSBO;
    glGenBuffers(1, &posInputSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posInputSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data), data, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, posInputSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


    glm::vec4 posOutputData[128 * 128];
    GLuint posOutputSSBO;
    glGenBuffers(1, &posOutputSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posOutputSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data), data, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, posOutputSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


    glm::vec4 velInputData[128 * 128];
    GLuint velInputSSBO;
    glGenBuffers(1, &velInputSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velInputSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(velInputData), velInputData, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, velInputSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glm::vec4 velOutputData[128 * 128];
    GLuint velOutputSSBO;
    glGenBuffers(1, &velOutputSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velOutputSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(velOutputData), velOutputData, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, velOutputSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    unsigned int MortonCodesData[128 * 128];
    GLuint mortonCodesSSBO;
    glGenBuffers(1, &mortonCodesSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mortonCodesSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(MortonCodesData), MortonCodesData, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, mortonCodesSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

App::~App()
{
    glfwTerminate();
}

void App::Run()
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);
    m_Clock.Restart();
    m_RenderProgram->Use();
    while (m_Window->Open())
    {
        float deltaTime = m_Clock.Stamp();

        // Processing user input
        ProcessEvents(deltaTime);

        // Rendering stuff goes here
        DoFrame(deltaTime);

        m_Menu->Render();

        PollEvents(deltaTime);
        m_Window->SwapBuffers();
    }
}

void App::DoFrame(float dt)
{
    if (m_RunSim)
    {
        // morton codes compute part
        m_MortonCodesComputeProgram->Use();
        m_PositionTextures[m_FrameCounter % 2]->BindCompute(1);
        //m_MortonCodesTexture->BindCompute(2);
        m_MortonCodesComputeProgram->SetInt("posImgInput", 1);
        //m_MortonCodesComputeProgram->SetInt("mortonCodesImg", 2);
        m_MortonCodesComputeProgram->SetFvec3("boundingBox", m_GeneralBoundingBox);
        glDispatchCompute(c_TextureSize / 8, c_TextureSize / 4, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // compute part
        m_ComputeProgram->Use();
       /* m_PositionTextures[m_FrameCounter % 2]->BindCompute(1);
        m_PositionTextures[(m_FrameCounter + 1) % 2]->BindCompute(2);
        m_VelocityTextures[m_FrameCounter % 2]->BindCompute(3);
        m_VelocityTextures[(m_FrameCounter + 1) % 2]->BindCompute(4);*/
        m_ComputeProgram->SetFloat("deltaTime", dt * m_SimulationSpeed);
      /*  m_ComputeProgram->SetInt("posImgInput", 1);
        m_ComputeProgram->SetInt("posImgOutput", 2);
        m_ComputeProgram->SetInt("velImgInput", 3);
        m_ComputeProgram->SetInt("velImgOutput", 4);*/
        m_ComputeProgram->SetFvec3("boundingBox", m_GeneralBoundingBox);
        std::cout << "hello" << std::endl;
       
        //glDispatchCompute(c_TextureSize / 8, c_TextureSize / 4, 1);
        glDispatchCompute(c_TextureSize / 8, c_TextureSize / 4, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }

    int bufferIndex = (m_RunSim ? m_FrameCounter : 0);
    // render part
    m_RenderProgram->Use();

    m_Texture->Bind(0);
    //m_PositionTextures[bufferIndex % 2]->Bind(1);
    //m_PositionTextures[(bufferIndex + 1) % 2]->Bind(2);
    //m_VelocityTextures[bufferIndex % 2]->Bind(3);
    //m_VelocityTextures[(bufferIndex + 1) % 2]->Bind(4);
    //m_RenderProgram->SetInt("u_Texture", 0);
    //m_RenderProgram->SetInt("u_TexturePos", 1);


    m_RenderProgram->SetMat4x4("u_ProjView", m_Camera->GetProjectionMatrix() * m_Camera->GetViewMatrix());
    m_RenderProgram->SetMat4x4("u_Model", glm::rotate(glm::identity<glm::mat4x4>(), (float)glfwGetTime(), glm::vec3(0, 1, 0)));
    //m_RenderProgram->SetMat4x4("u_Model", glm::identity<glm::mat4x4>());
    m_RenderProgram->SetMat4x4("u_CameraRotation", m_Camera->GetRotationMatrix());

    m_Window->Clear(0.05f, 0.05f, 0.1f);
    m_Mesh->Draw();
    m_FrameCounter++;
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
    if (glfwGetKey(m_Window->Get(), GLFW_KEY_TAB) == GLFW_PRESS)
        m_Mouse->EnableCursor(m_Window->Get());

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
