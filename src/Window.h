#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

class Window
{
public:
	Window(int width, int height, const char* title, bool fullscreen = false);
	~Window();

	GLFWwindow* Get() const;
	bool Open() const;
	float GetAspectRation() const;
	void SwapBuffers();
	void Clear(float r, float g, float b);
	void Close();
	void SetFullScreen(bool flag);

private:
	static void FramebufferSizeCallBack(GLFWwindow* window, int width, int height);

private:
	int m_Width;
	int m_Height;
	bool m_FullScreen;
	std::string m_Title;
	GLFWwindow* m_Window;
};

