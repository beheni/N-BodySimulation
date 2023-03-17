#pragma once
#include "SafeWindows.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class VertexBufferLayout 
{
public:
    struct VertexAttribute 
    {
        unsigned int count;
        unsigned int type;
        bool normalized;

        static unsigned int GetTypeSize(unsigned int type) 
        {
            switch (type) 
            {
            case GL_FLOAT: return sizeof(GLfloat);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
            }
            //TODO: add exception here - invalid attribute type specified
            return 0;
        }

        inline unsigned int GetSize() const { return count * GetTypeSize(type); }
    };

    VertexBufferLayout() : m_Stride(0) {}

    VertexBufferLayout& Push(unsigned int count, unsigned int type, bool normalized)
    {
        m_Attributes.push_back({ count, type, normalized });
        m_Stride += count * VertexAttribute::GetTypeSize(type);

        return *this;
    }

    void Bind() const
    {
        unsigned int offset = 0;
        for (size_t i = 0; i < m_Attributes.size(); i++)
        {
            auto& attr = m_Attributes[i];
            glVertexAttribPointer(i, attr.count, attr.type, attr.normalized, m_Stride, (void*)offset);
            glEnableVertexAttribArray(i);
            offset += attr.GetSize();
        }
    }

    inline const std::vector<VertexAttribute>& GetAttributes() const { return m_Attributes; }
    inline unsigned int GetStride() const { return m_Stride; }

private:
    std::vector<VertexAttribute> m_Attributes;
    unsigned int m_Stride;
};
