#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "log.h"

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

class Camera {
  public:
    glm::vec3 position{0, 0, 0};
    glm::vec3 front{0, 0, -1};
    glm::vec3 up{0, 1, 0};
    glm::vec3 right{1, 0, 0};
    glm::vec3 world_up{0, 1, 0};
    // euler Angles
    float yaw{YAW};
    float pitch{PITCH};
    // camera options
    float movement_speed{SPEED};
    float mouse_sensitivity{SENSITIVITY};
    float zoom{ZOOM};

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : position(position), up(up), yaw(yaw), pitch(pitch) {
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix() const { return glm::lookAt(position, position + front, up); }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
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

    void ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrainPitch = true) {
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

    void ProcessMouseScroll(float y_offset) {
        zoom -= y_offset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

  private:
    void updateCameraVectors() {
        glm::vec3 tmp_front{0, 0, 0};
        tmp_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        tmp_front.y = sin(glm::radians(pitch));
        tmp_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        tmp_front = glm::normalize(tmp_front);
        this->front = tmp_front;
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }
};
#endif
