#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "framebuffer.h"


Framebuffer::Framebuffer() : m_id(-1) {
    glGenFramebuffers(1, &m_id);
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &m_id);
}

void Framebuffer::activate() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

uint32_t Framebuffer::genRenderbuffer() {
    uint32_t rbo;
    glGenRenderbuffers(1, &rbo);
    return rbo;
}

void Framebuffer::setRenderbufferSize(uint32_t rbo, uint32_t width, uint32_t height) {
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
}

void Framebuffer::genAttachmentR(uint32_t rbo) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

void Framebuffer::genAttachmentT(TextureType type, uint32_t textureId, uint32_t faceId, uint32_t mipmapLevel) {
    if (type == TextureType::Cube) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceId, textureId, mipmapLevel);
    }
    else {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    }
}

void Framebuffer::reset() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
}