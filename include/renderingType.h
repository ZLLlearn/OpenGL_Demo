#ifndef RENDERINGTYPE_H
#define RENDERINGTYPE_H


enum class GraphicsAPI {
    OpenGL_4_5,
};

enum class StateType {
    Buffer,
    BackColor,
};

enum class CameraType {
    Ortho,
    Perspective,
};

enum class MoveDirection {
    Forward,
    Backward,
    Leftward,
    Rightward,
    Upward,
    Downward,
    Reset,
};

enum class TextureType {
    _2D,
    Cube,
};

enum class FrameAttachmentType {
    Texture,
    RenderBuffer,
};

#endif