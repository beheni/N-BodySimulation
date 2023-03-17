#pragma once
#include "ShaderBase.h"

#include <glad/glad.h>
#include <string>

class RenderShader : public ShaderBase
{
public:
	RenderShader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath);
	~RenderShader() override;

	void Use() const override;
};

