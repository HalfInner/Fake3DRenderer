//
// Created by kajbr on 28.02.2020.
//

#ifndef FAKE3DRENDERER_BASICCAMERA_HH
#define FAKE3DRENDERER_BASICCAMERA_HH

#include <glm/matrix.hpp>
#include <glm/ext.hpp>

struct /*interface*/ Camera {
    virtual glm::mat4 projection() = 0;
    virtual glm::mat4 view() = 0;
    virtual glm::mat4 model() = 0;
    virtual ~Camera() = default;
};

struct /*interface*/ Movable {
    enum class Direction {
        Forward, Backward, Leftward, Rightward, Upward, Downward
    };
    virtual void move(glm::vec3 vec) = 0;
    virtual void move(Direction direction, float elapsedTime) = 0;
    virtual ~Movable() = default;
};

struct /*interface*/ Rotatable {
    virtual void pitch(float angle) = 0;
    virtual void yaw(float angle) = 0;
    virtual void roll(float angle) = 0;
    virtual ~Rotatable() = default;
};

struct /*interface*/ Zoomable {
    virtual void zoom(float ratio) = 0;
    virtual ~Zoomable() = default;
};

class BasicCamera : public Camera {
  public:
    glm::mat4 projection() override;
    glm::mat4 view() override;
    glm::mat4 model() override;
};

class MovableCamera : public Camera, public Movable, public Rotatable, public Zoomable {
  public:
    MovableCamera();
    // Camera
    glm::mat4 projection() override;
    glm::mat4 view() override;
    glm::mat4 model() override;

    // Rotatable
    void pitch(float angle) override;
    void yaw(float angle) override;
    void roll(float angle) override;

    // Movable
    void move(glm::vec3 vec) override;
    void move(Direction direction, float elapsedTime) override;
    // Zoomable
    void zoom(float ratio) override;
  private:
    void updateCameraCoordinates();

    glm::vec3 position_{-1.0f, 0.0f, 4.0f};
    float cameraZoom_ {};

    float pitch_{.0f};
    float yaw_{-90.f};
    float roll_{ .0f};
    float velocity_{};
    glm::vec3 up_{};
    glm::vec3 front_{};
};

#endif //FAKE3DRENDERER_BASICCAMERA_HH
