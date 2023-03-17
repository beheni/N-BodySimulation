#pragma once
#include <glad/glad.h>
#include <string>

class RenderShader
{
public:
	RenderShader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath);
	~RenderShader();
	void Use();
private:
	GLuint m_ShaderProgram;
};

