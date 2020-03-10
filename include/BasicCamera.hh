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
    virtual glm::vec3 position() = 0;
    virtual void updateScreen(float width, float horizontal) = 0;
    virtual ~Camera() = default;
};

struct /*interface*/ Movable {
    enum class Direction {
        Forward, Backward, Leftward, Rightward, Upward, Downward
    };
    virtual void move(Direction direction, float elapsedTime) = 0;
    virtual ~Movable() = default;
};

struct /*interface*/ Rotatable {
    enum class HeadDirection {
        LeftSide, RightSide, UpSide, DownSide, LeftShoulder, RightShoulder
    };
    virtual void rotate(HeadDirection headDirection, float elapsedTime) = 0;

    virtual ~Rotatable() = default;
};

struct /*interface*/ Zoomable {
    enum class ResizeType {
        ZoomIn, ZoomOut
    };
    virtual void zoom(ResizeType resizeType, float elapsedTime) = 0;
    virtual ~Zoomable() = default;
};

class MovableCamera : public Camera, public Movable, public Rotatable, public Zoomable {
  public:
    MovableCamera();
    // Camera
    glm::mat4 projection() override;
    glm::mat4 view() override;
    glm::vec3 position() override;
    void updateScreen(float width, float height) override;

    // Rotatable
    void rotate(HeadDirection headDirection, float elapsedTime) override;

    // Movable
    void move(Direction direction, float elapsedTime) override;
    // Zoomable
    void zoom(ResizeType resizeType, float elapsedTime) override;
  private:
    void updateCameraCoordinates();
    glm::mat3 generateRotateMat(float yaw, float pitch, float roll);

    glm::vec3 position_{0.0f, 2.5f, 6.f};
    float cameraZoom_{};

    float pitch_{-15.0f};
    float yaw_{-90.f};
    float roll_{.0f};
    float velocity_{};
    glm::vec3 up_{};
    glm::vec3 front_{};
    float screenWidth_{800};
    float screenHeight_{640};
    glm::mat4 projection_;
    glm::mat4 view_;
    glm::vec3 target_;
    glm::vec3 direction_;
    glm::vec3 rigth_;
};

#endif //FAKE3DRENDERER_BASICCAMERA_HH
