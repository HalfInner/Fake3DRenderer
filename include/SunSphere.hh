//
// Created by kajbr on 08.03.2020.
//

#ifndef FAKE3DRENDERER_SUNSPHERE_HH
#define FAKE3DRENDERER_SUNSPHERE_HH

#include "NaiveSphere.hh"
namespace Graphic {

class SunSphere : public Graphic::Renderable, public Graphic::LightPoint, public Graphic::Animation {
  public:
    SunSphere() : naiveSphere_(0.25f, position_) {}

    // Renderable
    Graphic::RendererInfo beginDraw(float elapsedTime) override {
        auto info = naiveSphere_.beginDraw(elapsedTime);
        info.color = color_;
        info.typeObject = Utils::TypeObject::Light;

        if (isAnimationRunning_) {
            auto velocity = 1.5f;
            angle_ += elapsedTime * velocity;
        }
        auto rangeX = 5.f;
        auto rangeZ = 8.f;
        auto posAnimation = basePosition_;
        posAnimation.x += cos(angle_) * rangeX;
        posAnimation.z += sin(angle_) * rangeZ;

        position_ = posAnimation;
        info.position = position_;

        return info;
    }

    void initialize(std::shared_ptr<Graphic::Buffer> buffer) override {
        naiveSphere_.initialize(std::move(buffer));
    }

    void endDraw() override {
        naiveSphere_.endDraw();
    }

    // LightPoint
    glm::vec3 position() override {
        return position_;
    }

    glm::vec3 color() override {
        return color_;
    }

    float intensity() override {
        return intensity_;
    }

    void increaseIntensity(float elapsedTime) override {
        intensity_ += elapsedTime * backlightVelocity_;
        intensity_ = std::min(intensity_, 4.f);
    }

    void decreaseIntensity(float elapsedTime) override {
        intensity_ += -elapsedTime * backlightVelocity_;
        intensity_ = std::max(intensity_, 0.f);
    }

    // Animation
    void start() override {
        isAnimationRunning_ = true;
    }

    void stop() override {
        isAnimationRunning_ = false;
    }

  private:
    glm::vec3 basePosition_{-1.2f, 4.0f, -4.f};
    glm::vec3 position_{-1.2f, 4.0f, -4.f};
    glm::vec3 color_{1.f, 1.f, 1.f};
    bool isAnimationRunning_{false};
    float angle_{1.3f};
    float intensity_{1.f};
    float backlightVelocity_{2.f};

    NaiveSphere naiveSphere_;
};

} // namespace Graphic
#endif //FAKE3DRENDERER_SUNSPHERE_HH
