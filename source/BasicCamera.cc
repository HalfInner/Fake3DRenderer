//
// Created by kajbr on 28.02.2020.
//
#include "BasicCamera.hh"

#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <unordered_map>

MovableCamera::MovableCamera() {
    front_ = glm::vec3{0.0f, 0.0f, -1.0f}; // TODO (kaj) : What's that?
    cameraZoom_ = 45.f;
    velocity_ = 1.f;


    auto up = glm::vec3{0.f, 1.f, 0.f};
    up_ = generateRotateMat(0.f, 0.f, 0.f) * up;

    target_ = glm::vec3{0.f, 0.f, 0.f};
    direction_ = glm::normalize(glm::vec3{position_ - target_});
    rigth_ = glm::normalize(glm::cross(up_, direction_));

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
            roll_ += defaultAngle * velocity_ * elapsedTime;
            break;
        case HeadDirection::RightShoulder:
            roll_ += -defaultAngle * velocity_ * elapsedTime;
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
    updateCameraCoordinates();
}

void MovableCamera::zoom(ResizeType resizeType, float elapsedTime) {
    const float zoomRatio = resizeType == ResizeType::ZoomOut ? 90.f : -90.f;
    cameraZoom_ += zoomRatio * velocity_ * elapsedTime;
    cameraZoom_ = std::min(cameraZoom_, 100.f);
    cameraZoom_ = std::max(cameraZoom_, 10.f);
    updateCameraCoordinates();
}


void MovableCamera::updateCameraCoordinates() {

    front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_.y = sin(glm::radians(pitch_));
    front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

    projection_ = glm::perspective(glm::radians(cameraZoom_), screenWidth_ / screenHeight_, 0.1f, 100.0f);
    view_ = glm::lookAt(position_, position_ + front_, up_); // TODO (kaj) : Implement by yourself

    auto matViewTranslation = glm::mat4{
            rigth_.x, rigth_.y, rigth_.z, 0.f,
            up_.x, up_.y, up_.z, 0.f,
            direction_.x, direction_.y, direction_.z, 0.f,
            0.f, 0.f, 0.f, 0.f};
    auto matViewTranslationPosition = glm::mat4{
            1.f, 0.f, 0.f, -position_.x,
            0.f, 1.f, 0.f, -position_.y,
            0.f, 0.f, 1.f, -position_.z,
            0.f, 0.f, 0.f, 1.f};

    view_ = matViewTranslation * matViewTranslationPosition;
    std::cout << glm::to_string(view_) << std::endl;
    view_ = glm::lookAt(position_, position_ + front_, up_); // TODO (kaj) : Implement by yourself
    std::cout << glm::to_string(view_) << std::endl;
}

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

    return matRotateYaw * matRotatePitch * matRotateRoll;
}


