#include "ComputeShader.h"
#include "Exception.h"

#include <sstream>
#include <fstream>

ComputeShader::ComputeShader(const std::string& sourceFile)
{
    std::string shaderString;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        file.open(sourceFile);
        std::stringstream shaderStream;
        shaderStream << file.rdbuf();
        file.close();
        shaderString = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        throw EXCEPTION(e.what());
    }
    const char* cStringShader = shaderString.c_str();

    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &cStringShader, NULL);
    glCompileShader(shader);
    CheckCompilation(shader);

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, shader);
    glLinkProgram(m_ShaderProgram);
    CheckLinking(m_ShaderProgram);

    glDeleteShader(shader);
}

ComputeShader::~ComputeShader()
{
    glDeleteProgram(m_ShaderProgram);
}

void ComputeShader::Use() const
{
    glUseProgram(m_ShaderProgram);
}
