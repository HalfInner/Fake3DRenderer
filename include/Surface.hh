//
// Created by kajbr on 08.03.2020.
//

#ifndef FAKE3DRENDERER_SURFACE_HH
#define FAKE3DRENDERER_SURFACE_HH


#include <memory>
#include "GraphicBase.hh"
#include "Buffer.hh"

namespace Graphic {
class Surface : public Graphic::Renderable {
  public:
    void initialize(std::shared_ptr<Graphic::Buffer> buffer) override {
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
                            {0, 1, 2, 0, 2, 3}, Graphic::BufferType::Normal);
    }

    Graphic::RendererInfo beginDraw(float elapsedTime) override {
        Graphic::RendererInfo ri;
        ri.elements = 6;
        ri.color = color_;

        buffer_->bind();

        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Graphic::Buffer> buffer_;
    glm::vec3 color_{.0f, .53f, .27f};
};

} // namespace Graphic
#endif //FAKE3DRENDERER_SURFACE_HH
