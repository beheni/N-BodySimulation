#include "RenderProgram.h"
#include "Exception.h"

RenderProgram::RenderProgram(const std::string& vertexSourcePath, const std::string& fragmentSourcePath)
{
    std::string stringVertexSource;
    std::string stringFragmentSource;
    LoadShaderSource(vertexSourcePath, stringVertexSource);
    LoadShaderSource(fragmentSourcePath, stringFragmentSource);
    const char* cStringVertexSource = stringVertexSource.c_str();
    const char* cStringFragmentSource = stringFragmentSource.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &cStringVertexSource, NULL);
    glShaderSource(fragmentShader, 1, &cStringFragmentSource, NULL);

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
