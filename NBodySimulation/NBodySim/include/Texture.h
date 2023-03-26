#include <iostream>
#include <glad/glad.h>
#include <stb_image/stb_image.h>


class Texture {
public:
    unsigned int id;
    int width;
    int height;

    Texture(int w, int h);

    Texture(const char* filepath);

    ~Texture();

    void bind(unsigned int slot = 0) const;

    void unbind() const;
};
