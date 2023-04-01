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
        
        struct
        {
            float x;
            float y;
            float z;
        } translation;

        struct
        {
            float x;
            float y;
            float z;
        } position;

        struct 
        {
            float r;
            float g;
            float b;
        } color;

        struct 
        {
            float u;
            float v;
        } texCoord;
    };

    std::normal_distribution<float> dist(0, 50);
    std::default_random_engine eng;

    std::vector<Vertex> vertices;

    for (size_t i = 0; i < quadsNumber; i++)
    {
        glm::vec3 pos = { dist(eng), dist(eng), dist(eng) };
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
        .Push(3, GL_FLOAT, false) // translation
        .Push(3, GL_FLOAT, false) // pos
        .Push(3, GL_FLOAT, false) // col
        .Push(2, GL_FLOAT, false) //texture
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
