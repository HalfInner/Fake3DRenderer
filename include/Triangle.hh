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

class Triangle : public Renderable {
  public:
    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        buffer_->initialize({{-1, 0, 0},
                             {0,  1, 0},
                             {1,  0, 0}}, {0, 1, 2});
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
                             {1,  0,  0}}, {0, 1, 2});
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

class NaiveSphere : public Renderable {
  public:
    explicit NaiveSphere(float radius = 1.f, glm::vec3 position = {}) : radius_(radius), position_(position) {};

    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        std::vector<glm::vec3> spherePoints;
        float gamma = 0.f;
        float theta = 0.f;
        for (auto lstep = 0; lstep < latitudeSteps_; ++lstep) {
            theta = 0.f;
            gamma += 2 * M_PI / latitudeSteps_;
            for (auto mstep = 0.f; mstep < meridianSteps_; ++mstep) {
                theta += M_PI / meridianSteps_;
                auto pointX = radius_ * sin(theta) * cos(gamma);
                auto pointY = radius_ * sin(theta) * sin(gamma);
                auto pointZ = radius_ * cos(theta);

//                spherePoints.emplace_back(glm::normalize(glm::vec3{pointX, pointY, pointZ}));
                spherePoints.emplace_back(glm::vec3{pointX, pointY, pointZ});
            }
        }

        std::vector<unsigned> sphereIndicies;
        for (size_t i = 0; i < spherePoints.size(); ++i) {
            sphereIndicies.emplace_back(i % spherePoints.size());
            sphereIndicies.emplace_back((i + 1) % spherePoints.size());
            sphereIndicies.emplace_back((i + latitudeSteps_) % spherePoints.size());

            sphereIndicies.emplace_back((i + 1) % spherePoints.size());
            sphereIndicies.emplace_back((i + 1 + latitudeSteps_) % spherePoints.size());
            sphereIndicies.emplace_back((i + latitudeSteps_) % spherePoints.size());
        }

        buffer_->initialize(std::move(spherePoints), std::move(sphereIndicies));
    }

    RendererInfo beginDraw(float elapsedTime) override {
        RendererInfo ri;
        ri.elements = latitudeSteps_ * meridianSteps_ * 3 * 2;
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


    glm::vec3 color_{1.f, 0.84f, 0.01f};
    glm::vec3 position_;
};

class SunSphere : public Renderable, public LightPoint {
  public:
    SunSphere() : naiveSphere_(0.25f, position_) {}

    RendererInfo beginDraw(float elapsedTime) override {
        auto info = naiveSphere_.beginDraw(elapsedTime);
        info.color = color_;
        info.typeObject = RendererInfo::TypeObject::Light;

        return info;
    }

    void initialize(std::shared_ptr<Buffer> buffer) override {
        naiveSphere_.initialize(std::move(buffer));
    }

    void endDraw() override {
        naiveSphere_.endDraw();
    }

    glm::vec3 position() override {
        return position_;
    }

    glm::vec3 color() override {
        return color_;
    }

    float intensity() override {
        return 1.f;
    }

  private:
    glm::vec3 position_{-1, 2.5, 2};
    glm::vec3 color_ {1.f, 1.f, 1.f};
    NaiveSphere naiveSphere_;
};

class Cube : public Renderable {
  public:
    explicit Cube(glm::vec3 position = {}) :
            position_(position) {};

    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        auto sideLength = 0.5f;

        auto front_t_1 = glm::vec3{-sideLength, sideLength, sideLength};
        auto front_t_2 = glm::vec3{-sideLength, -sideLength, sideLength};
        auto front_t_3 = glm::vec3{sideLength, sideLength, sideLength};
        auto front_t_4 = glm::vec3{sideLength, -sideLength, sideLength};

        auto rear_t_1 = glm::vec3{-sideLength, sideLength, -sideLength};
        auto rear_t_2 = glm::vec3{-sideLength, -sideLength, -sideLength};
        auto rear_t_3 = glm::vec3{sideLength, sideLength, -sideLength};
        auto rear_t_4 = glm::vec3{sideLength, -sideLength, -sideLength};

        auto left_t_1 = glm::vec3{-sideLength, -sideLength, sideLength};
        auto left_t_2 = glm::vec3{-sideLength, -sideLength, -sideLength};
        auto left_t_3 = glm::vec3{-sideLength, sideLength, sideLength};
        auto left_t_4 = glm::vec3{-sideLength, sideLength, -sideLength};

        auto right_t_1 = glm::vec3{sideLength, -sideLength, sideLength};
        auto right_t_2 = glm::vec3{sideLength, -sideLength, -sideLength};
        auto right_t_3 = glm::vec3{sideLength, sideLength, sideLength};
        auto right_t_4 = glm::vec3{sideLength, sideLength, -sideLength};

        auto top_t_1 = glm::vec3{-sideLength, sideLength, sideLength};
        auto top_t_2 = glm::vec3{sideLength, sideLength, sideLength};
        auto top_t_3 = glm::vec3{-sideLength, sideLength, -sideLength};
        auto top_t_4 = glm::vec3{sideLength, sideLength, -sideLength};

        auto bottom_t_1 = glm::vec3{-sideLength, -sideLength, sideLength};
        auto bottom_t_2 = glm::vec3{sideLength, -sideLength, sideLength};
        auto bottom_t_3 = glm::vec3{-sideLength, -sideLength, -sideLength};
        auto bottom_t_4 = glm::vec3{sideLength, -sideLength, -sideLength};


        buffer_->initialize({front_t_1, front_t_2, front_t_3, front_t_4,
                             rear_t_1, rear_t_2, rear_t_3, rear_t_4,
                             left_t_1, left_t_2, left_t_3, left_t_4,
                             right_t_1, right_t_2, right_t_3, right_t_4,
                             top_t_1, top_t_2, top_t_3, top_t_4,
                             bottom_t_1, bottom_t_2, bottom_t_3, bottom_t_4},
                            {0, 1, 2, 1, 2, 3,
                             4, 5, 6, 6, 7, 5,
                             8, 10, 9, 10, 11, 9,
                             12, 13, 14, 13, 14, 15,
                             16, 17, 18, 17, 18, 19,
                             20, 21, 22, 21, 22, 23});
    }

    RendererInfo beginDraw(float elapsedTime) override {
        RendererInfo ri{};
        ri.elements = 6 * 2 * 3;
        ri.position = position_;
        ri.color = {0.2f, 0.2f, 0.9f};
        buffer_->bind();
        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_{nullptr};
    glm::vec3 position_;
};


} // namespace Graphic
#endif //FAKE3DRENDERER_TRIANGLE_HH
