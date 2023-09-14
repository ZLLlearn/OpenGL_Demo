#ifndef CAMERA_H
#define CAMERA_H
#include <memory>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "renderingType.h"

class Camera {
public:
    explicit Camera(CameraType type = CameraType::Perspective);
    ~Camera() = default;

    const glm::mat4& getViewMatrix() const;
    const glm::mat4& getProjMatrix() const;

    void pointRotate(float xoffset, float yoffset);
    void updateCameraParameters();
    void updateCameraPosition(MoveDirection direction, float offset);

private:
    void updateZoomScale(float offset);
    void updateMatrix();

    CameraType camera_type;
    glm::vec3 camera_position {};
    glm::vec3 camera_target {};
    glm::vec3 camera_up {};
    glm::vec3 camera_front{};
    glm::vec3 camera_right {};

    glm::mat4 viewMatrix {1.0f};
    glm::mat4 projMatrix {1.0f};

    float camera_fov {};
    float near_plane {};
    float far_plane {};
    float left_border {};
    float right_border {};
    float top_border {};
    float bottom_board {};

    float mouse_zoom {1.0f};
    glm::vec3 mouse_position {};
    std::shared_ptr<Camera> originCamere;

};



#endif