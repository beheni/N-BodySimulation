#include "RenderShader.h"
#include <sstream>
#include <fstream>

RenderShader::RenderShader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath)
{
    std::string vertexShaderString;
    std::string fragmentShaderString;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertexFile.open(vertexSourcePath);
        fragmentFile.open(fragmentSourcePath);
        std::stringstream vertexShaderStream;
        std::stringstream fragmentShaderStream;
        vertexShaderStream << vertexFile.rdbuf();
        fragmentShaderStream << fragmentFile.rdbuf();
        vertexFile.close();
        fragmentFile.close();
        vertexShaderString = vertexShaderStream.str();
        fragmentShaderString = fragmentShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        // throw error
    }
    const char* cStringVertexShader = vertexShaderString.c_str();
    const char* cStringFragmentShader = fragmentShaderString.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &cStringVertexShader, NULL);
    glShaderSource(fragmentShader, 1, &cStringFragmentShader, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vertexShader);
    glAttachShader(m_ShaderProgram, fragmentShader);
    glLinkProgram(m_ShaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

RenderShader::~RenderShader()
{
    glDeleteProgram(m_ShaderProgram);
}

void RenderShader::Use()
{
    glUseProgram(m_ShaderProgram);
}
