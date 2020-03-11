//
// Created by kajbr on 28.02.2020.
//
#include "BasicCamera.hh"

#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <unordered_map>

MovableCamera::MovableCamera() : velocity_(1.f), zoom_(45.f) {
    zoom_ = 45.f;


    updateCameraCoordinates();
}

glm::mat4 MovableCamera::projection() {
    return projection_;
}

glm::mat4 MovableCamera::view() {
    return view_;
}

glm::vec3 MovableCamera::position() {
    return position_;
}

void MovableCamera::updateScreen(float width, float height) {
    screenWidth_ = width;
    screenHeight_ = height;
    updateCameraCoordinates();
}

void MovableCamera::rotate(HeadDirection headDirection, float elapsedTime) {
    constexpr float defaultAngle = 90.f;
    switch (headDirection) {
        case HeadDirection::LeftSide:
            yaw_ += -defaultAngle * velocity_ * elapsedTime;
            break;
        case HeadDirection::RightSide:
            yaw_ += defaultAngle * velocity_ * elapsedTime;
            break;
        case HeadDirection::UpSide:
            pitch_ += defaultAngle * velocity_ * elapsedTime;
            break;
        case HeadDirection::DownSide:
            pitch_ += -defaultAngle * velocity_ * elapsedTime;
            break;
        case HeadDirection::LeftShoulder:
            dtRoll_ = defaultAngle * velocity_ * elapsedTime * 0.01f;
            roll_ += dtRoll_;
            break;
        case HeadDirection::RightShoulder:
            dtRoll_ = -defaultAngle * velocity_ * elapsedTime * 0.01f;
            roll_ += dtRoll_;
            break;
    }
    updateCameraCoordinates();
}

void MovableCamera::move(Direction direction, float elapsedTime) {
    // TODO (kaj): Consult that we always move forward from start point which is not intuitive
    const std::unordered_map<Direction, glm::vec3> vectorDirections{
            {Direction::Forward,   glm::vec3{0, 0, -1}},
            {Direction::Backward,  glm::vec3{0, 0, 1}},
            {Direction::Leftward,  glm::vec3{-1, 0, 0}},
            {Direction::Rightward, glm::vec3{1, 0, 0}},
            {Direction::Upward,    glm::vec3{0, 1, 0}},
            {Direction::Downward,  glm::vec3{0, -1, 0}}
    };

    float additionalMoveVelocity = 10.f;
    position_ += vectorDirections.at(direction) * velocity_ * additionalMoveVelocity * elapsedTime;
    target_ += vectorDirections.at(direction) * velocity_ * additionalMoveVelocity * elapsedTime;
    updateCameraCoordinates();
}

void MovableCamera::zoom(ResizeType resizeType, float elapsedTime) {
    const float zoomRatio = resizeType == ResizeType::ZoomOut ? 90.f : -90.f;
    zoom_ += zoomRatio * velocity_ * elapsedTime;
    zoom_ = std::min(zoom_, 100.f);
    zoom_ = std::max(zoom_, 10.f);
    updateCameraCoordinates();
}


void MovableCamera::updateCameraCoordinates() {

    front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_.y = sin(glm::radians(pitch_));
    front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    up_ = glm::vec3{0., 1.f, 0.f};

    projection_ = glm::perspective(glm::radians(zoom_), screenWidth_ / screenHeight_, 0.1f,
                                   100.0f); // TODO (kaj): Implement by yourself

    view_ = glm::lookAt(position_, position_ + front_, up_); // TODO (kaj) : Implement by yourself
    std::cout << glm::to_string(view_) << std::endl;
    view_ = generateView();
    std::cout << glm::to_string(view_) << std::endl;
}

glm::mat4 MovableCamera::generateView() {
    auto basicAxisX = glm::vec3{1.f, 0.f, 0.f};
    auto basicAxisY = glm::vec3{0.f, 1.f, 0.f};
    auto basicAxisZ = glm::vec3{0.f, 0.f, 1.f};
    (void)basicAxisX;
    (void)basicAxisY;
    (void)basicAxisZ;

    auto rotationMatrix = generateRotateMat(yaw_, pitch_, roll_);

    auto axisX = rotationMatrix * basicAxisX;
    auto axisY = rotationMatrix * basicAxisY;
    auto axisZ = rotationMatrix * basicAxisZ;

    auto matView = glm::mat4{
            axisX.x, axisY.x, axisZ.x, 0.f,
            axisX.y, axisY.y, axisZ.y, 0.f,
            axisX.z, axisY.z, axisZ.z, 0.f,
            -glm::dot(axisX, position_), -glm::dot(axisY, position_), -glm::dot(axisZ, position_), 1.f
    };
    return matView;
}

// pitch hang to up axisY
// yaw turn lef or right to axis X
// roll keep Z Axis unchenged
glm::mat3 MovableCamera::generateRotateMat(float yaw, float pitch, float roll) {
    yaw = glm::radians(yaw);
    glm::mat3 matRotateYaw{
            cosf(yaw), -sinf(yaw), 0.f,
            sinf(yaw), cosf(yaw), 0.f,
            0.f, 0.f, 1.f,
    };

    pitch = glm::radians(pitch);
    glm::mat3 matRotatePitch{
            cosf(pitch), 0.f, sinf(pitch),
            0.f, 1.f, 0.f,
            -sinf(pitch), 0.f, cosf(pitch),
    };

    pitch = glm::radians(roll);
    glm::mat3 matRotateRoll{
            1.f, 0.f, 0.f,
            0.f, cosf(roll), -sinf(roll),
            0.f, sinf(roll), cosf(roll)
    };
//    glm::mat4 matRotateYaw{
//            cosf(yaw), -sinf(yaw), 0.f, 0.f,
//            sinf(yaw), cosf(yaw), 0.f,0.f,
//            0.f, 0.f, 1.f,0.f,
//            0.f,0.f,0.f, 1.f
//    };
////         |  cos(A)  -sin(A)   0   0 |
////     M = |  sin(A)   cos(A)   0   0 |
////         |  0        0        1   0 |
////         |  0        0        0   1 |
//    pitch = glm::radians(pitch);
//    glm::mat4 matRotatePitch{
//            cosf(pitch), 0.f, sinf(pitch), 0.f,
//            0.f, 1.f, 0.f, 0.f,
//            -sinf(pitch), 0.f, cosf(pitch), 0.f,
//            0.f,0.f, 0.f, 1
//    };
////          |  cos(A)  0   sin(A)  0 |
////     M = |  0       1   0       0 |
////         | -sin(A)  0   cos(A)  0 |
////         |  0       0   0       1 |
//    pitch = glm::radians(roll);
//    glm::mat4 matRotateRoll{
//            1.f, 0.f, 0.f, 0,
//            0.f, cosf(roll), -sinf(roll), 0,
//            0.f, sinf(roll), cosf(roll), 0,
//            0, 0, 0, 1
//    };
////         |  1  0       0       0 |
////     M = |  0  cos(A) -sin(A)  0 |
////         |  0  sin(A)  cos(A)  0 |
////         |  0  0       0       1 |
    return matRotateYaw * matRotatePitch * matRotateRoll;
}


