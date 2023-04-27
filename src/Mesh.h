#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mesh
{
public:
	Mesh(size_t quadsNumber, float min, float max);
	~Mesh();

	void Draw();

private:
	GLuint m_VAO;
	GLuint m_VBO;

	size_t m_QuadsNumber;
};

