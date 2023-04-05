#pragma once
#include "SafeWindows.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

#include "RenderProgram.h"
#include "ComputeProgram.h"
#include "Exception.h"
#include "Window.h"
#include "Camera.h"
#include "Mouse.h""
#include "Clock.h"
#include "Mesh.h"
#include "Texture.h"

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
	Clock m_Clock;

	std::unique_ptr<RenderProgram> m_RenderProgram;
	std::unique_ptr<ComputeProgram> m_ComputeProgram;
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<Mouse> m_Mouse;
	std::unique_ptr<Mesh> m_Mesh;
	std::unique_ptr<Texture> m_Texture;
	std::unique_ptr<Texture> m_TexturePos;
};
