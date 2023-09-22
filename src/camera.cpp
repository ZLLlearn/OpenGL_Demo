#include <glm/gtx/quaternion.hpp>
#include "camera.h"


Camera::Camera(glm::vec3 position, CameraType type) : camera_position(std::move(position)), camera_type(type) {
    initParameters();
}

const glm::vec3& Camera::getPosition() const {
    return camera_position;
}

const glm::mat4& Camera::getViewMatrix() const {
    return viewMatrix;
}

const glm::mat4& Camera::getProjMatrix() const {
    return projMatrix;
}

void Camera::fitView(float width, float height) {
    bottomBoard = topBorder = width;
    rightBorder = leftBorder = height;
    updateMatrix();
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

void Camera::updatePosition(MoveDirection direction, float offset) {
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
        initParameters();
        break;
    default:
        break;
    }
    
    camera_target = camera_position + camera_front;
    updateMatrix();
}

void Camera::updateEuler(double xOffset, double yOffset) {
    float sensitivity = 0.08f;
    camera_yaw += xOffset * sensitivity;
    camera_pitch += yOffset * sensitivity;

    if (camera_pitch > 89.0f) {
        camera_pitch = 89.0f;
    }
    if (camera_pitch < -89.0f) {
        camera_pitch = -89.0f;
    }
    glm::vec3 front;
    front.x = cos(glm::radians(camera_yaw) * cos(glm::radians(camera_pitch)));
    front.y = sin(glm::radians(camera_pitch));
    front.z = sin(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
    camera_front = glm::normalize(front);
    camera_right = glm::normalize(glm::cross(camera_front, glm::vec3(0.0f, 1.0f, 0.0f)));
    camera_up = glm::normalize(glm::cross(camera_right, camera_front));
    camera_target = camera_front + camera_position;
    updateMatrix();
}

void Camera::updateZoomScale(double offset) {
    camera_fov += offset;
    if (camera_fov < 1.0f) {
        camera_fov = 1.0f;
    }
    else if (camera_fov > 45.0f) {
        camera_fov = 45.0f;
    }
    updateMatrix();
}

void Camera::initParameters() {
    camera_position = glm::vec3(0.0f, 0.0f, 10.0f);
    camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera_front = glm::normalize(camera_target - camera_position);
    camera_right = glm::normalize(glm::cross(camera_front, camera_up));
    camera_fov = 45.0f;
    camera_yaw = -90.0f;
    camera_pitch = 0.0f;
}

void Camera::updateParameters() {

}

void Camera::updateMatrix() {
    viewMatrix = glm::lookAt(camera_position, camera_target, camera_up);
    if (camera_type == CameraType::Ortho) {
        projMatrix = glm::ortho(leftBorder, rightBorder, bottomBoard, topBorder, nearPlane, farPlane);
    }
    else {
        projMatrix = glm::perspective(glm::radians(camera_fov), topBorder / leftBorder, nearPlane, farPlane);
    }
}

