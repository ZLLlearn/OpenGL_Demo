#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include "renderingType.h"

class Texture {
public:
    explicit Texture(TextureType type = TextureType::_2D);
    ~Texture();
    void activate(uint32_t unit = 0);
    void loadTexture(const char* filePath, bool mipmap = true);
    void genTexture(uint32_t width, uint32_t height, bool mipmap = false, uint32_t component = 3);
    uint32_t getId() {
        return m_id;
    }

private:
    uint32_t convertType(TextureType type);

    uint32_t m_id;
    TextureType m_type;
    void* m_data;

};


#endif