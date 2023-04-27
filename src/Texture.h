#include <iostream>
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>


class Texture {
public:
    Texture(int w, int h);
    Texture(int w, int h, glm::vec4* data);
    Texture(const char* filepath);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void BindCompute(unsigned int slot = 0) const;
    void Unbind() const;

private:
    GLuint m_TextureID;
    int m_Width;
    int m_Height;
    int m_NrChannels;
};
