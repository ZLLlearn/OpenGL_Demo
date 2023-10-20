#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "texture.h"


Texture::Texture(TextureType type) : m_id(0), m_type(type), m_data(nullptr) {
    glGenTextures(1, &m_id);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_id);
}

void Texture::activate(uint32_t unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(convertType(m_type), m_id);
}

void Texture::loadTexture(const char* filePath, bool mipmap) {
    stbi_set_flip_vertically_on_load(true);
    int width = 0, height = 0, nrComponents = 0;
    if (mipmap) {
        m_data = stbi_load(filePath, &width, &height, &nrComponents, 0);
    }
    else {
        m_data = stbi_loadf(filePath, &width, &height, &nrComponents, 0);
    }
    if (!m_data) {
        std::cout << "Failed to load image" << std::endl;
    }
    
    genTexture(width, height, mipmap, nrComponents);
    stbi_image_free(m_data);
}

void Texture::genTexture(uint32_t width, uint32_t height, bool mipmap, uint32_t component) {
    uint32_t format = 0;
    if (component == 1) {
        format = GL_RED;
    }
    else if (component == 2) {
        format = GL_RG;
    }
    else if (component == 3) {
        format = GL_RGB;
    }
    else if (component == 4) {
        format = GL_RGBA;
    }
    
    uint32_t type = convertType(m_type);
    glBindTexture(type, m_id);
    bool isLoad = m_data && mipmap;
    if (m_type == TextureType::_2D) {
        glTexImage2D(type, 0, format, width, height, 0, format, isLoad ? GL_UNSIGNED_BYTE : GL_FLOAT, m_data);
    }
    else {
        for (int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, format, GL_FLOAT, m_data);
        }
        glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    glTexParameteri(type, GL_TEXTURE_WRAP_S, isLoad ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, isLoad ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (!mipmap) {
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else {
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(type);
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

