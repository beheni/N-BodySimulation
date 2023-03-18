#include "Mesh.h"
#include "VertexBufferLayout.h"

Mesh::Mesh(int particlesCount)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    float vertices[] = 
    {
    -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0,
     0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0,
     0.5f,  0.5f, 0.0f, 0.0, 0.0, 1.0,

     0.5f,  0.5f, 0.0f, 0.0, 0.0, 1.0,
    -0.5f, 0.5f, 0.0f, 0.0, 1.0, 0.0,
     -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0
    };

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    VertexBufferLayout{}
        .Push(3, GL_FLOAT, false) // pos
        .Push(3, GL_FLOAT, false) // col
        .Bind();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_VAO);
}

void Mesh::Draw()
{
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
