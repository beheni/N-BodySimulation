#include "ShaderBase.h"
#include "Exception.h"

#include <sstream>
#include <fstream>

void ShaderBase::SetBool(const char* name, bool value)
{
	glUniform1i(FindUniformLocation(name), static_cast<int>(value));
}

void ShaderBase::SetInt(const char* name, int value)
{
	glUniform1i(FindUniformLocation(name), value);
}
//todo SetUint
void ShaderBase::SetFloat(const char* name, float value)
{
	glUniform1f(FindUniformLocation(name), value);
}

void ShaderBase::SetIntArray(const char* name, size_t count, const GLint* values)
{
	glUniform1iv(FindUniformLocation(name), count, values);
	//seems not to work
}

void ShaderBase::SetIvec3(const char* name, const glm::ivec3& value)
{
	glUniform3i(FindUniformLocation(name), value[0], value[1], value[2]);
}

void ShaderBase::SetFvec3(const char* name, const glm::vec3& value)
{
	glUniform3f(FindUniformLocation(name), value[0], value[1], value[2]);
}

void ShaderBase::SetMat4x4(const char* name, const glm::mat4x4& value)
{
	glUniformMatrix4fv(FindUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

GLint ShaderBase::FindUniformLocation(const char* name)
{
	GLint location = glGetUniformLocation(m_ShaderProgram, name);

	if (location == -1)
	{
		throw EXCEPTION(std::string("Uniform [") + name + "] location is not found");
	}

	return location;
}

void ShaderBase::CheckCompilation(GLuint shaderId) const
{
	GLint success{};
	char infoLog[512]{};

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		throw EXCEPTION(infoLog);
	}
}

void ShaderBase::CheckLinking(GLuint programId) const
{
	GLint success{};
	char infoLog[512]{};

	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, 512, NULL, infoLog);
		throw EXCEPTION(infoLog);
	}
}

void ShaderBase::LoadShaderSource(const std::string& sourcePath, std::string& sourceDest) const
{
	std::ifstream sourceFile;
	sourceFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	const char* cStringSource = nullptr;
	try
	{
		sourceFile.open(sourcePath);
		std::stringstream ss;
		ss << sourceFile.rdbuf();
		sourceFile.close();
		sourceDest = std::move(ss.str());
	} 
	catch (const std::exception& e)
	{
		std::string msg = "Failed to load shader source from [" + sourcePath + "].\n" + e.what();
		throw EXCEPTION(msg.c_str());
	}
}
