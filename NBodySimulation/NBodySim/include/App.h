#pragma once
#include "SafeWindows.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "RenderShader.h"
#include "Exception.h"
#include "Window.h"
#include "Camera.h"
#include "Clock.h"
#include "Mesh.h"

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

	std::unique_ptr<RenderShader> m_RenderShader;
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<Mesh> m_Mesh;
};
