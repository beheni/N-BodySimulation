#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mesh
{
public:
	Mesh(int particlesCount);
	~Mesh();

	void Draw();

private:
	GLuint m_VAO;
	GLuint m_VBO;
};

