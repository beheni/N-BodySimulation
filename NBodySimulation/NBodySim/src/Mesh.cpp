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
        Vertex(float x0, float y0, float z0, float x, float y, float z, float r, float g, float b, float u, float v)
            :
            translation({ x0, y0, z0 }), position({x, y, z}), color({r, g, b}), texCoord({u, v})
        {}
        
        glm::vec3 translation;
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texCoord;
    };

    std::normal_distribution<float> distX(0, 30);
    std::normal_distribution<float> distY(0, 2);
    std::normal_distribution<float> distZ(0, 30);
    std::default_random_engine eng;

    std::vector<Vertex> vertices;
    vertices.reserve(quadsNumber);

    for (size_t i = 0; i < quadsNumber; i++)
    {
        glm::vec3 pos = { distX(eng), distY(eng), distZ(eng)};
        vertices.emplace_back(pos.x, pos.y, pos.z, -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 0.0f, 0.0f);
        vertices.emplace_back(pos.x, pos.y, pos.z,  0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0f, 0.0f);
        vertices.emplace_back(pos.x, pos.y, pos.z,  0.5f,  0.5f, 0.0f, 0.0, 0.0, 1.0, 1.0f, 1.0f);
        vertices.emplace_back(pos.x, pos.y, pos.z,  0.5f,  0.5f, 0.0f, 0.0, 0.0, 1.0, 1.0f, 1.0f);
        vertices.emplace_back(pos.x, pos.y, pos.z, -0.5f,  0.5f, 0.0f, 0.0, 1.0, 0.0, 0.0f, 1.0f);
        vertices.emplace_back(pos.x, pos.y, pos.z, -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 0.0f, 0.0f);
    }

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    VertexBufferLayout{}
        .Push(3, GL_FLOAT, false) // global position
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
