#ifndef CAMERA_H
#define CAMERA_H

#include "glm/common.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
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
           float defPitch = 0.0f,
           glm::vec3 initialFocus = glm::vec3(0.0f));

    glm::mat4 getViewMatrix(){
        return glm::lookAt(pos, pos + front, up);
    }

    void processKeyboard(Movement direction, Key keyAction);

    void moveCamera(float deltaTime);

    void turnTo(glm::vec3 point3d = glm::vec3(0.0f));

    void orbitCamera(float xoffset, float yoffset);

    void processMouseMovement(float xoffset,
                              float yoffset, 
                              bool constrainPitch = true);

    void processMouseScroll(float yoffset);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
    std::array<bool, 6> movement {}; 
    float orbitRadius {4};
};

inline Camera::Camera(glm::vec3 initialPosition,
                      glm::vec3 cameraUp,
                      float defYaw,
                      float defPitch,
                      glm::vec3 initialFocus)
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
    turnTo(initialFocus);
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

inline void Camera::turnTo(glm::vec3 point3d) {
    float offset_x = pos.x - point3d.x;
    float offset_y = pos.y - point3d.y;
    float offset_z = pos.z - point3d.z;
    float offset_xz = glm::length(glm::vec2(offset_x, offset_z));

    if (offset_x == 0) {
        (offset_z > 0) ? yaw = -90.0f : yaw = 90.0f;
    }
    if (offset_x < 0) {
        yaw = atanf(offset_z/offset_x) * 180/glm::pi<float>();
    }
    else if (offset_x > 0) {
        if (offset_z < 0)
            yaw = atanf(offset_z/offset_x) * 180/glm::pi<float>() + 180.0f;
        else
            yaw = atanf(offset_z/offset_x) * 180/glm::pi<float>() - 180.0f;
    }
    pitch = atanf(-offset_y/offset_xz) * 180/glm::pi<float>();

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

inline void Camera::orbitCamera(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity * -0.1f;
    yoffset *= mouseSensitivity * -0.1f;

    pos += right * xoffset; 
    pos.y = pos.y + up.y * yoffset;

    if (pos.x > 0 && up.x*yoffset > 0){
        pos.x = pos.x + up.x * yoffset;
    }
    if (pos.z > 0 && up.z*yoffset > 0) {
        pos.z = pos.z + up.z * yoffset;
    }

    pos = orbitRadius * glm::normalize(pos);
    turnTo();
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
