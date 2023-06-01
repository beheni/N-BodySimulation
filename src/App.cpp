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
    m_BuildingTreeComputeProgram = std::make_unique<ComputeProgram>("./data/shaders/buildingTree.comp");
    m_TraversingTreeComputeProgram = std::make_unique<ComputeProgram>("./data/shaders/traversingTree.comp");
    m_Texture = std::make_unique<Texture>("./data/textures/star.png");
    m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 40.0f), 75.0f, m_Window->GetAspectRation(), 0.1f, 2000.0f);
    m_Mouse = std::make_unique<Mouse>(m_Window->Get());
    m_Menu = std::make_unique<Menu>("Menu", m_Window->Get());

    Menu::EmbraceTheDarkness();
    m_Menu->AddSliderFloat("Speed", &m_SimulationSpeed, 0.001f, 1.0f);
    m_Menu->AddCheckbox("Simulation", &m_RunSim);
    m_Menu->AddCheckbox("Rotation", &m_Rotate);
    m_Menu->AddButton("Enter FlyCam Mode", [this] {m_Mouse->DisableCursor(m_Window->Get()); });
    m_Menu->AddCombo("Start count sqrt", { "64", "128", "256", "512", "1024", "2048" }, &m_SimParams.selectedStarsCountStr, [this] {
            m_SimParams.starsCountSqrt = std::stoi(m_SimParams.selectedStarsCountStr);
        });
    m_Menu->AddSliderInt("Star mass", &m_SimParams.particleMass, 10, 10000);
    m_Menu->AddSliderInt("Build tree rate", &m_SimParams.buildTreeRate, 1, 10);
    m_Menu->AddButton("Regenerate", [this] {RegenerateState(); });

    RegenerateState();
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
    m_Time = (float)glfwGetTime();
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
    size_t starsCount = m_SimParams.starsCountSqrt * m_SimParams.starsCountSqrt;

    // render part
    int bufferIndex = (m_RunSim ? (m_FrameCounter) : 0);
    m_RenderProgram->Use();

    m_Texture->Bind(0);

    m_PositionBuffers[bufferIndex % 2]->Bind(1);
    m_PositionBuffers[(bufferIndex + 1) % 2]->Bind(2);

    m_Time = (m_Rotate ? m_Time + dt : m_Time);
    auto model = glm::rotate(glm::identity<glm::mat4x4>(), m_Time, glm::vec3(0, 1, 0));
    m_RenderProgram->SetMat4x4("u_Model", model);
    m_RenderProgram->SetMat4x4("u_ProjView", m_Camera->GetProjectionMatrix() * m_Camera->GetViewMatrix());

    m_Window->Clear(0.05f, 0.05f, 0.1f);
    m_Mesh->Draw();
    m_FrameCounter++;
    if (m_RunSim)
    {  
        size_t i = 0;
        if (m_FrameCounter % m_SimParams.buildTreeRate == 0) {

            m_ParticleIds[0]->SetData(particlesIds.data(), starsCount);
            m_ParticleIds[1]->SetData(particlesIds.data(), starsCount);

            // morton codes compute part
            m_MortonCodesComputeProgram->Use();
            m_PositionBuffers[m_FrameCounter % 2]->Bind(1);
            m_ParticleIds[0]->Bind(2);
            m_MortonCodesBuffers[0]->Bind(3);
            //m_MortonCodesBuffers[1]->Bind(4);
            m_MortonCodesComputeProgram->SetFvec3("u_BoundingBox", c_GeneralBoundingBox);
            glDispatchCompute(starsCount / 32, 1, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);


            // sorting morton codes
            m_SortingProgram->Use();
            m_MortonCodesBuffers[0]->Bind(1); // read
            m_MortonCodesBuffers[1]->Bind(2); // write
            m_ParticleIds[0]->Bind(3); // read
            m_ParticleIds[1]->Bind(4); // write
            for (size_t h = 2; h <= starsCount; h *= 2)
            {
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
                glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);
                m_SortingProgram->DoFlip(h);
                i++;
                m_MortonCodesBuffers[i % 2]->Bind(1); // read
                m_MortonCodesBuffers[(i + 1) % 2]->Bind(2); // write
                m_ParticleIds[i % 2]->Bind(3); // read
                m_ParticleIds[(i + 1) % 2]->Bind(4); // write
                for (size_t hh = h / 2; hh > 1; hh /= 2)
                {
                    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
                    glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);
                    m_SortingProgram->DoDisperse(hh);
                    i++;
                    m_MortonCodesBuffers[i % 2]->Bind(1); // read
                    m_MortonCodesBuffers[(i + 1) % 2]->Bind(2); // write
                    m_ParticleIds[i % 2]->Bind(3); // read
                    m_ParticleIds[(i + 1) % 2]->Bind(4); // write
                }
            }
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);

            // building tree compute part
            m_BuildingTreeComputeProgram->Use();
            m_BuildingTreeComputeProgram->SetInt("u_NumberOfParticlesSqrt", m_SimParams.starsCountSqrt);
            m_BuildingTreeComputeProgram->SetInt("u_ParticleMass", m_SimParams.particleMass);
            m_PositionBuffers[m_FrameCounter % 2]->Bind(1);
            m_ParticleIds[(i + 1) % 2]->Bind(2);
            m_MortonCodesBuffers[(i + 1) % 2]->Bind(3);
            m_TreeNodesBuffer->Bind(4);
            glDispatchCompute(starsCount / 32, 1, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        }
        // traversing tree compute part
        m_TraversingTreeComputeProgram->Use();
        m_TraversingTreeComputeProgram->SetFloat("u_DeltaTime", m_SimulationSpeed / 100.0f);
        m_TraversingTreeComputeProgram->SetFvec3("u_BoundingBox", c_GeneralBoundingBox);
        m_TraversingTreeComputeProgram->SetInt("u_NumberOfParticlesSqrt", m_SimParams.starsCountSqrt);
        m_TraversingTreeComputeProgram->SetInt("u_ParticleMass", m_SimParams.particleMass);
        m_PositionBuffers[m_FrameCounter        % 2]->Bind(1); // read
        m_PositionBuffers[(m_FrameCounter + 1)  % 2]->Bind(2); // write
        m_VelocityBuffers[m_FrameCounter        % 2]->Bind(3); // read
        m_VelocityBuffers[(m_FrameCounter + 1)  % 2]->Bind(4); // write
        m_ParticleIds[(i + 1) % 2]->Bind(5);
        m_TreeNodesBuffer->Bind(6);
        glDispatchCompute(starsCount / 32, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }
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

void App::RegenerateState()
{
    size_t starsCount = m_SimParams.starsCountSqrt * m_SimParams.starsCountSqrt;

    m_SortingProgram = std::make_unique<BitonicSort>("./data/shaders/bitonicSort.comp", starsCount);
    m_Mesh = std::make_unique<Mesh>(starsCount, -80, 80);
    std::vector<glm::vec4> data(starsCount);
    std::vector<glm::vec4> vel(starsCount);
    particlesIds.resize(starsCount);

#if 1
    std::normal_distribution<float> distX(0, 5);
    std::normal_distribution<float> distY(0, 1);
    std::normal_distribution<float> distZ(0, 5);
    std::default_random_engine eng(3);
#else
    std::uniform_real_distribution<float> distX(-50, 50);
    std::uniform_real_distribution<float> distY(-50, 50);
    std::uniform_real_distribution<float> distZ(-50, 50);
    std::default_random_engine eng(4);
#endif
    for (size_t i = 0; i < starsCount; i++)
    {
        glm::vec3 pos = { distX(eng), distY(eng), distZ(eng) };
        data[i] = glm::vec4(pos.x, pos.y, pos.z, 0.0f);

        float scale = sqrt(pos.x * pos.x + pos.z * pos.z);// / (10.0f * (float(rand() % 50 + 1) / 10));

        vel[i] = glm::vec4(pos.z / scale, 0.0f, -pos.x / scale, 0.0f);
        particlesIds[i] = i;
    }
    m_PositionBuffers.clear();
    m_VelocityBuffers.clear();
    m_MortonCodesBuffers.clear();
    m_ParticleIds.clear();

    m_PositionBuffers.emplace_back(std::make_unique<SSBO<glm::vec4>>(data.data(), data.size()));
    m_PositionBuffers.emplace_back(std::make_unique<SSBO<glm::vec4>>(data.data(), data.size()));
    m_VelocityBuffers.emplace_back(std::make_unique<SSBO<glm::vec4>>(vel.data(), vel.size()));
    m_VelocityBuffers.emplace_back(std::make_unique<SSBO<glm::vec4>>(vel.data(), vel.size()));

    m_MortonCodesBuffers.emplace_back(std::make_unique<SSBO<unsigned int>>(starsCount));
    m_MortonCodesBuffers.emplace_back(std::make_unique<SSBO<unsigned int>>(starsCount));
    m_ParticleIds.emplace_back(std::make_unique<SSBO<unsigned int>>(particlesIds.data(), starsCount));
    m_ParticleIds.emplace_back(std::make_unique<SSBO<unsigned int>>(particlesIds.data(), starsCount));

    std::vector<TreeNode_t> treeNodesSSBO(2 * (starsCount) - 1);
    m_TreeNodesBuffer = std::make_unique<SSBO<TreeNode_t>>(treeNodesSSBO.data(), treeNodesSSBO.size());
}
