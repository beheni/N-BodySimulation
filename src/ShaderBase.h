#pragma once
#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <string>

class ShaderBase
{
public:
	ShaderBase() = default;

	virtual void Use() const = 0;
	virtual ~ShaderBase() = default;

	void SetBool(const char* name, bool value);
	void SetInt(const char* name, int value);
	void SetFloat(const char* name, float value);
	void SetIntArray(const char* name, size_t count, const GLint* values);
	void SetMat4x4(const char* name, const glm::mat4x4& value);
	void SetIvec3(const char* name, const glm::ivec3& value);
	void SetFvec3(const char* name, const glm::vec3& value);

protected:

	GLint FindUniformLocation(const char* name);
	void CheckCompilation(GLuint shaderId) const;
	void CheckLinking(GLuint programId) const;
	void LoadShaderSource(const std::string& sourcePath, std::string& sourceDest) const;

protected:
	GLuint m_ShaderProgram;
};
