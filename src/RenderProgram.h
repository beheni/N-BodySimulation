#pragma once
#include "ShaderBase.h"

#include <glad/glad.h>
#include <string>

class RenderProgram : public ShaderBase
{
public:
	RenderProgram(const std::string& vertexSourcePath, const std::string& fragmentSourcePath);
	~RenderProgram() override;

	void Use() const override;
};

