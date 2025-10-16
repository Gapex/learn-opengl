#include "../include/Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position), up(up), yaw(yaw), pitch(pitch) {
    updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movement_speed * deltaTime;
    if (direction == FORWARD) {
        position += front * velocity;
    }
    if (direction == BACKWARD) {
        position -= front * velocity;
    }
    if (direction == LEFT) {
        position -= right * velocity;
    }
    if (direction == RIGHT) {
        position += right * velocity;
    }
    if (direction == UP) {
        position += up * velocity;
    }
    if (direction == DOWN) {
        position -= up * velocity;
    }
}

void Camera::ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrainPitch) {
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw += x_offset;
    pitch -= y_offset;
    if (constrainPitch) {
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float y_offset) {
    zoom -= y_offset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    glm::vec3 tmp_front{0, 0, 0};
    tmp_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    tmp_front.y = sin(glm::radians(pitch));
    tmp_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    tmp_front = glm::normalize(tmp_front);
    this->front = tmp_front;
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}
