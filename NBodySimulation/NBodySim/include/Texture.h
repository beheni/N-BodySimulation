#include <iostream>
#include <glad/glad.h>
#include <stb_image/stb_image.h>


class Texture {
public:
    Texture(int w, int h);
    Texture(const char* filepath);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

private:
public:
    GLuint m_TextureID;
    int m_Width;
    int m_Height;
    int m_NrChannels;
};
