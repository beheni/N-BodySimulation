#pragma once
#include <glad/glad.h>
#include <glm/matrix.hpp>

class ShaderBase
{
public:
	ShaderBase() = default;

	virtual void Use() const = 0;
	virtual ~ShaderBase() = default;

	void SetBool(const char* name, bool value);
	void SetInt(const char* name, int value);
	void SetFloat(const char* name, float value);
	void SetMat4x4(const char* name, const glm::mat4x4& value);

protected:
	void CheckCompilation(GLuint shaderId) const;
	void CheckLinking(GLuint programId) const;

protected:
	GLuint m_ShaderProgram;
};

