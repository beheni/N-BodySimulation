#pragma once
#include <glad/glad.h>
#include "ComputeProgram.h"

#include <string>
#include <memory>

class BitonicSort
{
	enum Algorithm
	{
		LocalBMS = 0,
		BigFlip = 1,
		LocalDisperse = 2,
		BigDisperse = 3,
	};
public:
	BitonicSort(const std::string& shaderFile);

	void Sort(int n);

private:
	void DispatchLocalBMS(uint32_t n, uint32_t h);
	void DispatchBigFlip(uint32_t n, uint32_t h);
	void DispatchLocalDisperse(uint32_t n, uint32_t h);
	void DispatchBigDisperse(uint32_t n, uint32_t h);

	void Dispatch(uint32_t n, uint32_t h);

private:
	GLint m_MaxComputeSharedMemorySize = 0;
	GLint m_MaxComputeWorkGroupSize = 2;
	GLuint m_WorkGroupCount = 0;

	std::unique_ptr<ComputeProgram> m_ComputeProgram;
};

