//
// Created by kajbr on 28.02.2020.
//
#include "BasicCamera.hh"

#include <unordered_map>

MovableCamera::MovableCamera() {
    updateCameraCoordinates();

    up_ = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraZoom_ = 45.f;

    velocity_ = 1.f;
}

glm::mat4 MovableCamera::projection() {
    return projection_;
}

glm::mat4 MovableCamera::view() {
    return view_;d
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
    if (cameraZoom_ < 10.f)
        cameraZoom_ = 10.f;
    if (cameraZoom_ > 100.f)
        cameraZoom_ = 100.f;
    updateCameraCoordinates();
}

void MovableCamera::updateCameraCoordinates() {
    front_ = glm::vec3(0.0f, 0.0f, -1.0f); // TODO (kaj) : What's that?
    front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_.y = sin(glm::radians(pitch_));
    front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));


    projection_ = glm::perspective(glm::radians(cameraZoom_), screenWidth_ / screenHeight_, 0.1f, 100.0f);
    view_ = glm::lookAt(position_, position_ + front_, up_); // TODO (kaj) : Implement by yourself
}


