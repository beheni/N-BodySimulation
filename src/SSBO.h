#pragma once
#include <glad/glad.h>

template <typename Type>
class SSBO
{
public:
	SSBO() = default;
	SSBO(unsigned int size);
	SSBO(Type* data, unsigned int size);
	~SSBO();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

private:
	GLuint m_BufferID;
};

template<typename Type>
SSBO<Type>::SSBO(unsigned int size)
{
	Type* data = new Type[size]{};
	glGenBuffers(1, &m_BufferID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Type) * size, data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	delete[] data;
}

template<typename Type>
SSBO<Type>::SSBO(Type* data, unsigned int size)
{
	glGenBuffers(1, &m_BufferID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Type) * size, data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

template<typename Type>
SSBO<Type>::~SSBO()
{
	glDeleteBuffers(1, &m_BufferID);
}

template<typename Type>
void SSBO<Type>::Bind(unsigned int slot) const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_BufferID);
}

template<typename Type>
void SSBO<Type>::Unbind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
