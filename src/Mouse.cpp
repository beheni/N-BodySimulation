#include "Mouse.h"

Mouse::Mouse(GLFWwindow* window)
{
	PlaceInCenter(window);
	m_Fixed = false;
}

void Mouse::DisableCursor(GLFWwindow* window)
{
	PlaceInCenter(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_Fixed = true;
}

void Mouse::EnableCursor(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_Fixed = false;
}

glm::vec2 Mouse::GetOffset(GLFWwindow* window) const
{
	double mouseX = 0;
	double mouseY = 0;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glm::vec2 offset = glm::vec2(static_cast<float>(mouseX), static_cast<float>(mouseY)) - m_SavedPosition;
	
	if (m_Fixed)
	{
		glfwSetCursorPos(window, static_cast<double>(m_SavedPosition.x), static_cast<double>(m_SavedPosition.y));
	}
	else
	{
		m_SavedPosition.x = mouseX;
		m_SavedPosition.y = mouseY;

		return glm::vec2(0.0f, 0.0f);
	}

	return offset;
}

void Mouse::PlaceInCenter(GLFWwindow* window)
{
	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);
	m_SavedPosition.x = static_cast<float>(width / 2);
	m_SavedPosition.y = static_cast<float>(height / 2);
	glfwSetCursorPos(window, static_cast<double>(m_SavedPosition.x), static_cast<double>(m_SavedPosition.y));
}
