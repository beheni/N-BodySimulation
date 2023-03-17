#pragma once
#include <glad/glad.h>

class ShaderBase
{
public:
	ShaderBase() = default;

	virtual void Use() const = 0;
	virtual ~ShaderBase() = default;

protected:
	void CheckCompilation(GLuint shaderId) const;
	void CheckLinking(GLuint programId) const;

protected:
	GLuint m_ShaderProgram;
};

