#pragma once
#include <glad/glad.h>
#include <string>

class ComputeShader
{
public:
	ComputeShader(const std::string& sourceFile);
	~ComputeShader();

	void Use();

private:
	GLuint m_ShaderProgram;
};

