//
// Created by kajbr on 28.02.2020.
//

#include "BasicCamera.hh"

glm::mat4 BasicCamera::projection() {
    auto cameraZoom = 45.f;
    glm::mat4 projection = glm::perspective(glm::radians(cameraZoom), (float)800 / (float)640, 0.1f, 100.0f);
    return projection;
}

glm::mat4 BasicCamera::view() {
    auto position = glm::vec3(-1.0f, 0.0f, 4.0f);
    auto up = glm::vec3(0.0f, 1.0f, 0.0f);
    auto front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::mat4 view = glm::lookAt(position, position + front, up);
    return view;
}

glm::mat4 BasicCamera::model() {
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, {0,0,0});
    float angle = 20.0f * 1;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    return model;
}

glm::mat4 MovableCamera::projection() {
    cameraZoom_ = 45.f;
    glm::mat4 projection = glm::perspective(glm::radians(cameraZoom_), (float)800 / (float)640, 0.1f, 100.0f);
    return projection;
}

glm::mat4 MovableCamera::view() {
    auto up = glm::vec3(0.0f, 1.0f, 0.0f); // TODO (kaj) : What's that?
    auto front = glm::vec3(0.0f, 0.0f, -1.0f); // TODO (kaj) : What's that?
    glm::mat4 view = glm::lookAt(position_, position_ + front, up); // TODO (kaj) : Implement by yourself
    return view;
}

glm::mat4 MovableCamera::model() {
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, {0,0,0});
    float angle = 20.0f * 1;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    return model;
}

void MovableCamera::move(glm::vec3 vec) {
    velocity_ = 0.0000004f;
    position_ += vec * velocity_;
}

void MovableCamera::zoom(float ratio) {
    cameraZoom_ = ratio;
}

void MovableCamera::pitch(float angle) {
    pitch_ = angle;
}

void MovableCamera::yaw(float angle) {
    yaw_ = angle;
}

void MovableCamera::roll(float angle) {
    roll_ = angle;
}
