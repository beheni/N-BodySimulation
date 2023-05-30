#pragma once
#include "SafeWindows.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

#include "RenderProgram.h"
#include "ComputeProgram.h"
#include "BitonicSort.h"
#include "Exception.h"
#include "Window.h"
#include "Camera.h"
#include "Mouse.h"
#include "Clock.h"
#include "Mesh.h"
#include "Texture.h"
#include "Menu.h"
#include "SSBO.h"

class App
{
public:
	App();
	~App();

	void Run();

private:
	void DoFrame(float dt);
	void ProcessEvents(float dt);
	void PollEvents(float dt);

private:
	const glm::vec3 c_GeneralBoundingBox = {100.0f, 100.0f, 100.0f};
	const size_t c_NumberParticlesSqrt = 64;
	const size_t c_ParticleMass = 30000;

	float m_SimulationSpeed = 1.0f;
	bool m_RunSim = false;
	bool m_Rotate = false;

	float m_Time;
	Clock m_Clock;
	size_t m_FrameCounter = 0;

	#pragma pack(4)
	struct TreeNode_t 
	{
		glm::vec4 centerOfMass;
		float totalMass;
		glm::ivec2 children;
		float boundingBoxMetric;
	};

	
	std::unique_ptr<Menu> m_Menu;
	std::unique_ptr<RenderProgram> m_RenderProgram;
	std::unique_ptr<ComputeProgram> m_MortonCodesComputeProgram;
	std::unique_ptr<ComputeProgram> m_BuildingTreeComputeProgram;
	std::unique_ptr<ComputeProgram> m_TraversingTreeComputeProgram;
	std::unique_ptr<ComputeProgram> m_ComputeProgram;
	std::unique_ptr<BitonicSort> m_SortingProgram;
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<Mouse> m_Mouse;
	std::unique_ptr<Mesh> m_Mesh;
	std::unique_ptr<Texture> m_Texture;

	std::vector<std::unique_ptr<SSBO<glm::vec4>>> m_PositionBuffers;
	std::vector<std::unique_ptr<SSBO<glm::vec4>>> m_VelocityBuffers;
	std::vector<std::unique_ptr<SSBO<unsigned int>>> m_MortonCodesBuffer;
	std::vector<std::unique_ptr<SSBO<unsigned int>>> m_ParticleIds;
	std::unique_ptr<SSBO<TreeNode_t>> m_TreeNodesBuffer;
};
