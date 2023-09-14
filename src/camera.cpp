#include <glm/gtx/quaternion.hpp>
#include "camera.h"


Camera::Camera(CameraType type) : camera_type(type), camera_position(0.0f, 0.0f, 1.0f), camera_target(0.0f, 0.0f, 0.0f), camera_up(0.0f, 1.0f, 0.0f) {
    
}

const glm::mat4& Camera::getViewMatrix() const {
    return viewMatrix;
}

const glm::mat4& Camera::getProjMatrix() const {
    return projMatrix;
}

void Camera::pointRotate(float xoffset, float yoffset) {
    //auto func = [](glm::vec3& pos, const glm::vec3& center, const glm::quat& q) {
    //    pos -= center;
    //    pos *= glm::mat3_cast(q);
    //    pos += center;
    //};
    //glm::quat q{0, 0, 0, 1};
    //q = glm::angleAxis(yoffset, camera_right);
    //q *= glm::angleAxis(xoffset, camera_up);
    //func(camera_position, camera_target, q);
    //func(camera_target, camera_target, q);
    //camera_front = glm::normalize(camera_target - camera_position);
    //camera_up = glm::normalize(glm::cross(camera_front, camera_right));
}

void Camera::updateCameraParameters() {
    camera_front = glm::normalize(camera_target - camera_position);
    camera_right = glm::normalize(glm::cross(camera_front, camera_up));
}

void Camera::updateCameraPosition(MoveDirection direction, float offset) {
    switch (direction)
    {
    case MoveDirection::Forward:
        camera_position += camera_front * offset;
        break;
    case MoveDirection::Backward:
        camera_position -= camera_front * offset;
        break;
    case MoveDirection::Leftward:
        camera_position -= camera_right * offset;
        break;
    case MoveDirection::Rightward:
        camera_position += camera_right * offset;
        break;
    case MoveDirection::Upward:
        camera_position += camera_up * offset;
        break;
    case MoveDirection::Downward:
        camera_position -= camera_up * offset;
        break;
    case MoveDirection::Reset:
        camera_position = originCamere->camera_position;
        break;
    default:
        break;
    }
}

void Camera::updateZoomScale(float offset) {
    camera_fov += offset;
    if (camera_fov < 1.0f) {
        camera_fov = 1.0f;
    }
    else if (camera_fov > 45.0f) {
        camera_fov = 45.0f;
    }
}

void Camera::updateMatrix() {
    viewMatrix = glm::lookAt(camera_position, camera_target, camera_up);
    if (camera_type == CameraType::Ortho) {
        projMatrix = glm::ortho(left_border, right_border, bottom_board, top_border, near_plane, far_plane);
    }
    else {
        projMatrix = glm::perspective(glm::radians(camera_fov), top_border / left_border, near_plane, far_plane);
    }
}

