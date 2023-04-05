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
        Vertex(float idx, float idy, float x, float y, float z, float r, float g, float b, float u, float v)
            :
            id({idx, idy}), position({ x, y, z }), color({ r, g, b }), texCoord({ u, v })
        {}
        
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texCoord;
        glm::vec2 id;
    };

    std::vector<Vertex> vertices;
    vertices.reserve(quadsNumber);

    for (int i = 0; i < quadsNumber; i++)
    {
        glm::vec2 id = {float(i % 1024) / 1024.0f, float(i / 1024) / 1024.0f};
        vertices.emplace_back(id.x, id.y, -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 0.0f, 0.0f);
        vertices.emplace_back(id.x, id.y,  0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0f, 0.0f);
        vertices.emplace_back(id.x, id.y,  0.5f,  0.5f, 0.0f, 0.0, 0.0, 1.0, 1.0f, 1.0f);
                             
        vertices.emplace_back(id.x, id.y,  0.5f,  0.5f, 0.0f, 0.0, 0.0, 1.0, 1.0f, 1.0f);
        vertices.emplace_back(id.x, id.y, -0.5f,  0.5f, 0.0f, 0.0, 1.0, 0.0, 0.0f, 1.0f);
        vertices.emplace_back(id.x, id.y, -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 0.0f, 0.0f);
    }

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    VertexBufferLayout{}
        .Push(3, GL_FLOAT, false) // local position
        .Push(3, GL_FLOAT, false) // color
        .Push(2, GL_FLOAT, false) // texCoords
        .Push(2, GL_FLOAT, false) // id
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
