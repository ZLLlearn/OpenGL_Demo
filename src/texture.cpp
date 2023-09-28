#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "texture.h"


Texture::Texture(TextureType type) : m_id(0), m_type(type) {
    glGenTextures(1, &m_id);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_id);
}

void Texture::activate(uint32_t unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(convertType(m_type), m_id);
}

void Texture::loadTexture(const char* filePath) {
    stbi_set_flip_vertically_on_load(true);
    int width = 0, height = 0, nrComponents = 0;
    float* data = stbi_loadf(filePath, &width, &height, &nrComponents, 0);
    if (!data) {
        std::cout << "Failed to load image" << std::endl;
    }
    genTexture(width, height, data);
    stbi_image_free(data);
}

void Texture::genTexture(uint32_t width, uint32_t height, void* data, bool hasRgb, bool mipmap) {
    activate();
    uint32_t rgbType = GL_RGB;
    if (!hasRgb) {
        rgbType = GL_RG;
    }
    if (m_type == TextureType::_2D) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, rgbType, GL_FLOAT, data);
    }
    else {
        for (int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, rgbType, GL_FLOAT, data);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    uint32_t type = convertType(m_type);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (mipmap) {
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
    else {
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
}

uint32_t Texture::convertType(TextureType type) {
    if (type == TextureType::_2D) {
        return GL_TEXTURE_2D;
    }
    else {
        return GL_TEXTURE_CUBE_MAP;
    }
}

