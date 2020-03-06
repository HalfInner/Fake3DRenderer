//
// Created by kajbr on 23.02.2020.
//

#ifndef FAKE3DRENDERER_TRIANGLE_HH
#define FAKE3DRENDERER_TRIANGLE_HH

#include <cmath>

#include "Renderable.hh"
#include "Buffer.hh"

namespace Graphic {


struct /*interface*/ RendererInfo {
    enum class TypeObject {
        Normal, Light, NotSpecified
    };

    enum RenderType : unsigned {
        Triangles = GL_TRIANGLES,
        Lines = GL_LINES,
        UnknownRenderType = 0x0
    };

    enum CountType : unsigned {
        Byte = GL_UNSIGNED_BYTE,
        Short = GL_UNSIGNED_SHORT,
        Integer = GL_UNSIGNED_INT,
        UnknownCountType = 0x0
    };
//    TypeObject typeObject = TypeObject::Normal;
    TypeObject typeObject = TypeObject::NotSpecified;
    RenderType renderType = RenderType::Triangles;
    CountType countType = CountType::Integer;
    unsigned elements = 0u;

    glm::vec3 position{};
    glm::vec3 color{};

    bool debugMode = false;
};

struct /*interface*/ Renderable {
    virtual void initialize(std::shared_ptr<Buffer> buffer) = 0;
    virtual RendererInfo beginDraw(float elapsedTime) = 0;
    virtual void endDraw() = 0;
    virtual ~Renderable() = default;
};

struct /*interface*/ LightPoint {
    virtual glm::vec3 position() = 0;
    virtual glm::vec3 color() = 0;
    virtual float intensity() = 0;
    virtual ~LightPoint() = default;
};

struct /*interface*/ Animation {
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ~Animation() = default;
};

class Triangle : public Renderable {
  public:
    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        buffer_->initialize({{-1, 0, 0},
                             {0,  1, 0},
                             {1,  0, 0}}, {0, 1, 2}, false);
    }

    RendererInfo beginDraw(float elapsedTime) override {
        RendererInfo ri;
        ri.elements = 1;

        buffer_->bind();

        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_;
};

class TriangleInv : public Renderable {
  public:
    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        buffer_->initialize({{-1, 0,  0},
                             {0,  -1, 0},
                             {1,  0,  0}}, {0, 1, 2}, false);
    }

    RendererInfo beginDraw(float elapsedTime) override {
        RendererInfo ri;
        ri.elements = 1;

        buffer_->bind();

        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_;
};

class Surface : public Renderable {
  public:
    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        auto sideLenght = 100.f;
        auto levelHeight = -.5f;
        auto normal = glm::vec3{0, 1, 0};
        buffer_->initialize({
                                    {sideLenght, levelHeight, sideLenght}, normal,
                                    {-sideLenght, levelHeight, sideLenght}, normal,
                                    {-sideLenght, levelHeight, -sideLenght}, normal,
                                    {sideLenght, levelHeight, -sideLenght}, normal,
                            },
                            {0, 1, 2, 0, 2, 3}, true);
    }

    RendererInfo beginDraw(float elapsedTime) override {
        RendererInfo ri;
        ri.elements = 6;
        ri.color = color_;

        buffer_->bind();

        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_;
    glm::vec3 color_{.0f, .53f, .27f};
};

class NaiveSphere : public Renderable {
  public:
    explicit NaiveSphere(float radius = 1.f, glm::vec3 position = {}, glm::vec3 color = {1.f, 0.9f, 0.9f})
            : radius_(radius),
              position_(position),
              color_(color) {};

    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        std::vector<glm::vec3> spherePoints{};
        float gamma = 0.f;
        for (auto lstep = 0; lstep < latitudeSteps_; ++lstep) {
            float theta = 0.f;
            gamma += 2 * M_PI / latitudeSteps_;
            for (auto mstep = 0.f; mstep < meridianSteps_; ++mstep) {
                theta += M_PI / meridianSteps_;
                auto pointX = radius_ * sin(theta) * cos(gamma);
                auto pointY = radius_ * sin(theta) * sin(gamma);
                auto pointZ = radius_ * cos(theta);

                spherePoints.emplace_back(glm::vec3{pointX, pointY, pointZ});
                spherePoints.emplace_back(glm::normalize(glm::vec3{pointX, pointY, pointZ}));
            }
        }

        std::vector<unsigned> sphereIndicies{};
        for (size_t i = 0; i < spherePoints.size(); ++i) {
            sphereIndicies.emplace_back(i % spherePoints.size());
            sphereIndicies.emplace_back((i + 1) % spherePoints.size());
            sphereIndicies.emplace_back((i + latitudeSteps_) % spherePoints.size());

            sphereIndicies.emplace_back((i + 1) % spherePoints.size());
            sphereIndicies.emplace_back((i + 1 + latitudeSteps_) % spherePoints.size());
            sphereIndicies.emplace_back((i + latitudeSteps_) % spherePoints.size());
        }

        buffer_->initialize(std::move(spherePoints), std::move(sphereIndicies), true);
    }

    RendererInfo beginDraw(float elapsedTime) override {
        RendererInfo ri;

//        fillPercentage_ += elapsedTime / 10000000.f;
//        if (fillPercentage_ > 99.f) {
//            fillPercentage_ = 84.f;
//        }
//
//        ri.elements =  numberOfElements * fillPercentage_;
//        ri.elements =  numberOfElements / 2;
        ri.elements = numberOfElements;
        ri.position = position_;

        // TODO(kaj): Example animation. To remove.
//        const float velocity_ = 0.00008f;
//        ri.color = color_ + (lastColor_ * cosf(elapsedTime) * velocity_);
//        lastColor_ = ri.color;
        ri.color = color_;
        ri.debugMode = false;

        buffer_->bind();

        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_;
    const float radius_ = 2.f;
    const int latitudeSteps_ = 50;
    const int meridianSteps_ = 50;

    const int numberOfElements = latitudeSteps_ * meridianSteps_ * 3 * 2;


    glm::vec3 position_{};
    glm::vec3 color_{};
};

class SunSphere : public Renderable, public LightPoint, public Animation {
  public:
    SunSphere() : naiveSphere_(0.25f, position_) {}

    // Renderable
    RendererInfo beginDraw(float elapsedTime) override {
        auto info = naiveSphere_.beginDraw(elapsedTime);
        info.color = color_;
        info.typeObject = RendererInfo::TypeObject::Light;

        auto posAnimation = basePosition_;
        if (isAnimationRunning_) {
            auto rangeX = 4.f;
            auto rangeZ = 4.f;
            auto velocity = 1.5f;
            angle_ += elapsedTime * velocity;
            posAnimation.x += cos(angle_) * rangeX;
            posAnimation.z += sin(angle_) * rangeZ;
        }
        position_ = posAnimation;
        info.position = position_;

        return info;
    }

    void initialize(std::shared_ptr<Buffer> buffer) override {
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
        return 1.f;
    }

    // Animation
    void start() override {
        isAnimationRunning_ = true;
    }

    void stop() override {
        isAnimationRunning_ = false;
    }

  private:
    glm::vec3 basePosition_{-1.2f, 4.0f, -1.f};
    glm::vec3 position_{-1.2f, 4.0f, -1.f};
    glm::vec3 color_{1.f, 1.f, 1.f};
    NaiveSphere naiveSphere_;
    bool isAnimationRunning_{true};
    float angle_{};
};

class Cuboid : public Renderable {
  public:
    explicit Cuboid(glm::vec3 position = {}, glm::vec3 sideLengths = {1.f, 1.f, 1.f},
                    glm::vec3 color = {1.0f, 0.5f, 0.31f}) :
            position_(position), sideLengths_(sideLengths), color_(color) {};

    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        auto sidePosX = sideLengths_.x / 2;
        auto sidePosY = sideLengths_.y / 2;
        auto sidePosZ = sideLengths_.z / 2;

        auto front_t_1 = glm::vec3{-sidePosX, sidePosY, sidePosZ};
        auto front_t_2 = glm::vec3{-sidePosX, -sidePosY, sidePosZ};
        auto front_t_3 = glm::vec3{sidePosX, sidePosY, sidePosZ};
        auto front_t_4 = glm::vec3{sidePosX, -sidePosY, sidePosZ};
        auto front_norm_ = glm::vec3{0, 0, 1};

        auto rear_t_1 = glm::vec3{-sidePosX, sidePosY, -sidePosZ};
        auto rear_t_2 = glm::vec3{-sidePosX, -sidePosY, -sidePosZ};
        auto rear_t_3 = glm::vec3{sidePosX, sidePosY, -sidePosZ};
        auto rear_t_4 = glm::vec3{sidePosX, -sidePosY, -sidePosZ};
        auto rear_norm_ = glm::vec3{0, 0, -1};

        auto left_t_1 = glm::vec3{-sidePosX, -sidePosY, sidePosZ};
        auto left_t_2 = glm::vec3{-sidePosX, -sidePosY, -sidePosZ};
        auto left_t_3 = glm::vec3{-sidePosX, sidePosY, sidePosZ};
        auto left_t_4 = glm::vec3{-sidePosX, sidePosY, -sidePosZ};
        auto left_norm_ = glm::vec3{-1, 0, 0};

        auto right_t_1 = glm::vec3{sidePosX, -sidePosY, sidePosZ};
        auto right_t_2 = glm::vec3{sidePosX, -sidePosY, -sidePosZ};
        auto right_t_3 = glm::vec3{sidePosX, sidePosY, sidePosZ};
        auto right_t_4 = glm::vec3{sidePosX, sidePosY, -sidePosZ};
        auto right_norm_ = glm::vec3{1, 0, 0};

        auto top_t_1 = glm::vec3{-sidePosX, sidePosY, sidePosZ};
        auto top_t_2 = glm::vec3{sidePosX, sidePosY, sidePosZ};
        auto top_t_3 = glm::vec3{-sidePosX, sidePosY, -sidePosZ};
        auto top_t_4 = glm::vec3{sidePosX, sidePosY, -sidePosZ};
        auto top_norm_ = glm::vec3{0, 1, 0};

        auto bottom_t_1 = glm::vec3{-sidePosX, -sidePosY, sidePosZ};
        auto bottom_t_2 = glm::vec3{sidePosX, -sidePosY, sidePosZ};
        auto bottom_t_3 = glm::vec3{-sidePosX, -sidePosY, -sidePosZ};
        auto bottom_t_4 = glm::vec3{sidePosX, -sidePosY, -sidePosZ};
        auto bottom_norm_ = glm::vec3{0, -1, 0};


        buffer_->initialize({
                                    front_t_1, front_norm_, front_t_2, front_norm_, front_t_3, front_norm_, front_t_4,
                                    front_norm_,
                                    rear_t_1, rear_norm_, rear_t_2, rear_norm_, rear_t_3, rear_norm_, rear_t_4,
                                    rear_norm_,
                                    left_t_1, left_norm_, left_t_2, left_norm_, left_t_3, left_norm_, left_t_4,
                                    left_norm_,
                                    right_t_1, right_norm_, right_t_2, right_norm_, right_t_3, right_norm_, right_t_4,
                                    right_norm_,
                                    top_t_1, top_norm_, top_t_2, top_norm_, top_t_3, top_norm_, top_t_4, top_norm_,
                                    bottom_t_1, bottom_norm_, bottom_t_2, bottom_norm_, bottom_t_3, bottom_norm_,
                                    bottom_t_4,
                                    bottom_norm_,
                            }, {
                                    0, 1, 2, 1, 2, 3,
                                    4, 5, 6, 6, 7, 5,
                                    8, 10, 9, 10, 11, 9,
                                    12, 13, 14, 13, 14, 15,
                                    16, 17, 18, 17, 18, 19,
                                    20, 21, 22, 21, 22, 23}, true);
    }

    RendererInfo beginDraw(float elapsedTime) override {
        RendererInfo ri{};
        ri.elements = 6 * 2 * 3;
        ri.position = position_;
        ri.color = color_;
        buffer_->bind();
        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_{nullptr};
    glm::vec3 position_;
    glm::vec3 sideLengths_;
    glm::vec3 color_;
};


} // namespace Graphic
#endif //FAKE3DRENDERER_TRIANGLE_HH
