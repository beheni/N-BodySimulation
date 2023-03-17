#include "ShaderBase.h"
#include "Exception.h"

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
