#ifndef CAMERA_H
#define CAMERA_H

#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace sjd {
class Camera {
public:
    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    glm::vec3 up;
    glm::vec3 worldUp;
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 right;

    float pitch {};
    float yaw   {};

    float movementSpeed {};
    float mouseSensitivity {};
    float zoom {};


    Camera(glm::vec3 initialPosition = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f),
           float defYaw = -90.0f,
           float defPitch = 0.0f);

    glm::mat4 getViewMatrix(){
        return glm::lookAt(pos, pos + front, up);
    }

    void processKeyboard(Movement direction, float deltaTime);

    void processMouseMovement(float xoffset,
                              float yoffset, 
                              GLboolean constrainPitch = true);

    void processMouseScroll(float yoffset);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

};

inline Camera::Camera(glm::vec3 initialPosition,
                      glm::vec3 cameraUp,
                      float defYaw,
                      float defPitch)
: up        {cameraUp}
, worldUp   {up}
, pos       {initialPosition}
, front     {glm::vec3(0.0f, 0.0f, -1.0f)}
, right     {}
, pitch     {defPitch}
, yaw       {defYaw}
, movementSpeed {2.5f}
, mouseSensitivity {0.1f}
, zoom {45.0f}
{
    updateCameraVectors();
}

inline void Camera::processKeyboard(Camera::Movement direction,
                                    float deltaTime) {
    float velocity {movementSpeed * deltaTime};
    if (direction == FORWARD)
        pos += glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * velocity;
    if (direction == BACKWARD)
        pos -= glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * velocity;
    if (direction == LEFT)
        pos -= right * velocity;
    if (direction == RIGHT)
        pos += right * velocity;
    if (direction == UP)
        pos += worldUp * velocity;
    if (direction == DOWN)
        pos -= worldUp * velocity;
}

inline void Camera::processMouseMovement(float xoffset,
                                         float yoffset,
                                         GLboolean constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

inline void Camera::processMouseScroll(float yoffset) {
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

inline void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 newFront {};
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up    = glm::normalize(glm::cross(right, front));
}

}
#endif
