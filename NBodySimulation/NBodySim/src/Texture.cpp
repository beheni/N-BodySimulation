#include "Texture.h"
#include <string>

Texture::Texture(int w, int h) 
    : 
    m_Width(w), m_Height(h)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    //glGenerateMipmap(GL_TEXTURE_2D);
    glBindImageTexture(0, m_TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    Unbind();
}

Texture::Texture(const char* filepath) 
{
    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load(filepath, &m_Width, &m_Height, &m_NrChannels, 0);
    std::string filepathStr(filepath);
    bool isPng = filepathStr.substr(filepathStr.size() - 3, 3) == "png";
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, (isPng ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        throw std::runtime_error("Failed to load texture: " + std::string(filepath));
        return;
    }        
    stbi_image_free(data);
}

Texture::~Texture() 
{
    glDeleteTextures(1, &m_TextureID);
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
