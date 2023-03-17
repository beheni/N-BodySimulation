#pragma once
#include "ShaderBase.h"

#include <glad/glad.h>
#include <string>

class ComputeShader : public ShaderBase
{
public:
	ComputeShader(const std::string& sourceFile);
	~ComputeShader() override;

	void Use() const override;
};

