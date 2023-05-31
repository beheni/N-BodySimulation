#pragma once
#include <glad/glad.h>
#include "ComputeProgram.h"

#include <string>
#include <memory>

class BitonicSort
{
	enum Algorithm
	{
		Flip = 0,
		Disperse = 1,
	};
public:
	BitonicSort(const std::string& shaderFile, size_t numberOfElements);

	void Use();
	void Sort();


	void DoFlip(uint32_t h);
	void DoDisperse(uint32_t h);

	void Dispatch(uint32_t h);

private:
	GLint m_MaxComputeSharedMemorySize = 0;
	GLint m_MaxComputeWorkGroupSize = 2;
	GLuint m_WorkGroupCount = 0;

	uint32_t m_NumberOfElemetes;

	std::unique_ptr<ComputeProgram> m_ComputeProgram;
};

