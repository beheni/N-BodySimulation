#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Mouse
{
public:
	Mouse(GLFWwindow* window);

	void DisableCursor(GLFWwindow* window);
	void EnableCursor(GLFWwindow* window);
	glm::vec2 GetOffset(GLFWwindow* window) const;

private:
	void PlaceInCenter(GLFWwindow* window);

private:
	mutable glm::vec2 m_SavedPosition;
	bool m_Fixed;
};

