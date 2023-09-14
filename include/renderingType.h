#ifndef RENDERINGTYPE_H
#define RENDERINGTYPE_H


enum class GraphicsAPI {
    OpenGL_4_5,
};

enum class StateType {
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

#endif