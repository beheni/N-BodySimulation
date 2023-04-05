#include "Mesh.h"
#include "VertexBufferLayout.h"

#include <glm/glm.hpp>
#include <random>

Mesh::Mesh(size_t quadsNumber, float min, float max)
    :
    m_QuadsNumber(quadsNumber)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    struct Vertex
    {
        Vertex() = default;
        Vertex(int ID, float x, float y, float z, float r, float g, float b, float u, float v)
            :
            ID(ID), position({x, y, z}), color({r, g, b}), texCoord({u, v})
        {}
        
        int ID;
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texCoord;
    };

    std::vector<Vertex> vertices;
    vertices.reserve(quadsNumber);

    for (size_t i = 0; i < quadsNumber; i++)
    {
        vertices.emplace_back(i, -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 0.0f, 0.0f);
        vertices.emplace_back(i,  0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0f, 0.0f);
        vertices.emplace_back(i,  0.5f,  0.5f, 0.0f, 0.0, 0.0, 1.0, 1.0f, 1.0f);
                             
        vertices.emplace_back(i,  0.5f,  0.5f, 0.0f, 0.0, 0.0, 1.0, 1.0f, 1.0f);
        vertices.emplace_back(i, -0.5f,  0.5f, 0.0f, 0.0, 1.0, 0.0, 0.0f, 1.0f);
        vertices.emplace_back(i, -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 0.0f, 0.0f);
    }

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    VertexBufferLayout{}
        .Push(1, GL_INT,   false) // ID
        .Push(3, GL_FLOAT, false) // local position
        .Push(3, GL_FLOAT, false) // color
        .Push(2, GL_FLOAT, false) // texCoords
        .Bind();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_VAO);
}

void Mesh::Draw()
{
	glDrawArrays(GL_TRIANGLES, 0, 6 * m_QuadsNumber);
}
