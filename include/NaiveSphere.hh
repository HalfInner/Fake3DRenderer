//
// Created by kajbr on 08.03.2020.
//

#ifndef FAKE3DRENDERER_NAIVESPHERE_HH
#define FAKE3DRENDERER_NAIVESPHERE_HH

namespace Graphic {

#include "BasicCamera.hh"
#include "Buffer.hh"

class NaiveSphere : public Graphic::Renderable {
  public:
    explicit NaiveSphere(float radius = 1.f, glm::vec3 position = {}, glm::vec3 color = {1.f, 0.9f, 0.9f})
            : radius_(radius),
              position_(position),
              color_(color) {};

    void initialize(std::shared_ptr<Graphic::Buffer> buffer) override {
        buffer_ = std::move(buffer);


        // TODO(kaj) : Fix 'Pac Man' effect
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

        buffer_->initialize(std::move(spherePoints), std::move(sphereIndicies), Graphic::BufferType::Normal);
    }

    Graphic::RendererInfo beginDraw(float elapsedTime) override {
        Graphic::RendererInfo ri;

        ri.elements = numberOfElements;
        ri.position = position_;
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

} // namespace Graphics
#endif //FAKE3DRENDERER_NAIVESPHERE_HH
