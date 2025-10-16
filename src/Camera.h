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
constexpr float SENSITIVITY = 0.05f;
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

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    [[nodiscard]] glm::mat4 GetViewMatrix() const { return glm::lookAt(position, position + front, up); }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    void ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrainPitch = true);

    void ProcessMouseScroll(float y_offset);

  private:
    void updateCameraVectors();
};
#endif
