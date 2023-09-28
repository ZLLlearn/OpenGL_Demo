#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "renderingType.h"


class Framebuffer {
public:
    Framebuffer();
    ~Framebuffer();

    void activate();
    uint32_t genRenderbuffer();
    void setRenderbufferSize(uint32_t rbo, uint32_t width, uint32_t height);
    void genAttachmentR(uint32_t rbo);
    void genAttachmentT(TextureType type, uint32_t textureId, uint32_t faceId = 0, uint32_t mipmapLevel = 0);

    static void reset();
private:
    uint32_t m_id;

};


#endif