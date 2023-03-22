#include "RenderProgram.h"
#include "Exception.h"

#include <sstream>
#include <fstream>

RenderProgram::RenderProgram(const std::string& vertexSourcePath, const std::string& fragmentSourcePath)
{
    std::string vertexShaderString;
    std::string fragmentShaderString;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

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

    const char* cStringVertexShader = vertexShaderString.c_str();
    const char* cStringFragmentShader = fragmentShaderString.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &cStringVertexShader, NULL);
    glShaderSource(fragmentShader, 1, &cStringFragmentShader, NULL);

    glCompileShader(vertexShader);
    CheckCompilation(vertexShader);
    glCompileShader(fragmentShader);
    CheckCompilation(fragmentShader);

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vertexShader);
    glAttachShader(m_ShaderProgram, fragmentShader);
    glLinkProgram(m_ShaderProgram);
    CheckLinking(m_ShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

RenderProgram::~RenderProgram()
{
    glDeleteProgram(m_ShaderProgram);
}

void RenderProgram::Use() const
{
    glUseProgram(m_ShaderProgram);
}
