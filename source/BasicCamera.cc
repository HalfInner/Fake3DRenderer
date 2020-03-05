//
// Created by kajbr on 28.02.2020.
//
#include "BasicCamera.hh"

#include <unordered_map>


glm::mat4 BasicCamera::projection() {
    auto cameraZoom = 45.f;
    glm::mat4 projection = glm::perspective(glm::radians(cameraZoom), (float) 800 / (float) 640, 0.1f, 100.0f);
    return projection;
}

glm::mat4 BasicCamera::view() {
    auto position = glm::vec3(-1.0f, 0.0f, 4.0f);
    auto up = glm::vec3(0.0f, 1.0f, 0.0f);
    auto front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::mat4 view = glm::lookAt(position, position + front, up);
    return view;
}

//glm::mat4 BasicCamera::model() {
//    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    model = glm::translate(model, {0, 0, 0});
//    float angle = 20.0f * 1;
//    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//    return model;
//}

MovableCamera::MovableCamera() {
    updateCameraCoordinates();

    up_ = glm::vec3(0.0f, 1.0f, 0.0f); // TODO (kaj) : What's that?
    cameraZoom_ = 45.f;

    velocity_ = 0.0000008f;
}

void MovableCamera::updateCameraCoordinates() {
    front_ = glm::vec3(0.0f, 0.0f, -1.0f); // TODO (kaj) : What's that?
    front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_.y = sin(glm::radians(pitch_));
    front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
}

glm::mat4 MovableCamera::projection() {
    glm::mat4 projection = glm::perspective(glm::radians(cameraZoom_), (float) 800 / (float) 640, 0.1f, 100.0f);
    return projection;
}

glm::mat4 MovableCamera::view() {
    glm::mat4 view = glm::lookAt(position_, position_ + front_, up_); // TODO (kaj) : Implement by yourself
    return view;
}

//glm::mat4 MovableCamera::model() {
//    return model;
//}

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
    position_ += vectorDirections.at(direction) * velocity_  * additionalMoveVelocity * elapsedTime;
}

void MovableCamera::zoom(float ratio) {
    cameraZoom_ += ratio * velocity_;
}

void MovableCamera::zoom(ResizeType resizeType, float elapsedTime) {
    const float zoomRatio = resizeType == ResizeType::ZoomOut ? 90.f : -90.f;
    cameraZoom_ += zoomRatio * velocity_ * elapsedTime;
    if (cameraZoom_ < 10.f)
        cameraZoom_ = 10.f;
    if (cameraZoom_ > 100.f)
        cameraZoom_ = 100.f;
}

void MovableCamera::pitch(float angle) {
    pitch_ += angle * velocity_;
    updateCameraCoordinates();
}

void MovableCamera::yaw(float angle) {
    yaw_ += angle * velocity_;
    updateCameraCoordinates();
}

void MovableCamera::roll(float angle) {
    roll_ += angle * velocity_;
    updateCameraCoordinates();
}

void MovableCamera::rotate(HeadDirection headDirection, float elapsedTime) {
    float defaultAngle = 90.f;
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

glm::vec3 MovableCamera::position() {
    return position_;
}



