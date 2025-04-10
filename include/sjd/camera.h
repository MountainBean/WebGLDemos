#ifndef CAMERA_H
#define CAMERA_H

#include <array>
#include <bitset>
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

    enum Key {
        RELEASE,
        PRESS
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
    bool firstMouse {true};
    float lastX;
    float lastY;


    Camera(glm::vec3 initialPosition = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f),
           float defYaw = -90.0f,
           float defPitch = 0.0f);

    glm::mat4 getViewMatrix(){
        return glm::lookAt(pos, pos + front, up);
    }

    void processKeyboard(Movement direction, Key keyAction);

    void moveCamera(float deltaTime);

    void processMouseMovement(float xoffset,
                              float yoffset, 
                              bool constrainPitch = true);

    void processMouseScroll(float yoffset);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
    std::array<bool, 6> movement {}; 
    // forward, backward, left, right, up, down
    //
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
, movementSpeed {5.f}
, mouseSensitivity {0.1f}
, zoom {45.0f}
{
    updateCameraVectors();
}

inline void Camera::processKeyboard(Camera::Movement direction,
                                    Key keyAction) {
    if (direction == FORWARD) {
        movement[FORWARD] = static_cast<bool>(keyAction);
    }
    if (direction == BACKWARD) {
        movement[BACKWARD] = static_cast<bool>(keyAction);
    }
    if (direction == LEFT) {
        movement[LEFT] = static_cast<bool>(keyAction);
    }
    if (direction == RIGHT) {
        movement[RIGHT] = static_cast<bool>(keyAction);
    }
    if (direction == UP) {
        movement[UP] = static_cast<bool>(keyAction);
    }
    if (direction == DOWN) {
        movement[DOWN] = static_cast<bool>(keyAction);
    }
}

inline void Camera::moveCamera(float deltaTime) {
    float velocity {movementSpeed * deltaTime};
    if (movement[FORWARD])
        pos += glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * velocity;
    if (movement[BACKWARD])
        pos -= glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * velocity;
    if (movement[LEFT])
        pos -= right * velocity;
    if (movement[RIGHT])
        pos += right * velocity;
    if (movement[UP])
        pos += worldUp * velocity;
    if (movement[DOWN])
        pos -= worldUp * velocity;
}

inline void Camera::processMouseMovement(float xoffset,
                                         float yoffset,
                                         bool constrainPitch) {
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
