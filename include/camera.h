#ifndef CAMERA_H
#define CAMERA_H
#include <memory>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "renderingType.h"

class Camera {
public:
    Camera(CameraType type = CameraType::Perspective, glm::vec3 position);
    ~Camera() = default;

    const glm::mat4& getViewMatrix() const;
    const glm::mat4& getProjMatrix() const;

    void fitView(float width, float height);
    void pointRotate(float xoffset, float yoffset);
    void updatePosition(MoveDirection direction, float offset);
    void updateEuler(double xOffset, double yOffset);
    void updateZoomScale(double offset);

private:
    void initParameters();
    void updateParameters();
    void updateMatrix();

    CameraType camera_type;
    glm::vec3 camera_position {};
    glm::vec3 camera_target {};
    glm::vec3 camera_up {};
    glm::vec3 camera_front{};
    glm::vec3 camera_right {};

    glm::mat4 viewMatrix {1.0f};
    glm::mat4 projMatrix {1.0f};

    float camera_fov {45.0f};
    float nearPlane {0.1f};
    float farPlane {500.0f};
    float leftBorder {};
    float rightBorder {};
    float topBorder {};
    float bottomBoard {};

    float camera_yaw{-90.0f};
    float camera_pitch{0.0f};

    float camera_zoom {1.0f};
    glm::vec3 mouse_position {};

};



#endif