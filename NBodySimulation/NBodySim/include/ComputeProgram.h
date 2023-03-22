#pragma once
#include "ShaderBase.h"

#include <glad/glad.h>
#include <string>

class ComputeProgram : public ShaderBase
{
public:
	ComputeProgram(const std::string& sourceFile);
	~ComputeProgram() override;

	void Use() const override;
};

