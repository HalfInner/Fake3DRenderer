//
// Created by kajbr on 23.02.2020.
//

#ifndef FAKE3DRENDERER_TRIANGLE_HH
#define FAKE3DRENDERER_TRIANGLE_HH

#include "Renderable.hh"
#include "Buffer.hh"

namespace Graphic {

struct /*interface*/ Renderable {
    virtual void initialize(std::shared_ptr<Buffer> buffer) = 0;
    virtual void beginDraw() = 0;
    virtual void endDraw() = 0;
    virtual ~Renderable() = default;
};

class Triangle : public Renderable {
  public:
    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        buffer_->initialize({{-1, 0,  0},
                             {0,   1, 0},
                             {1,  0,  0}}, {0, 1, 2});
    }

    void beginDraw() override {
        buffer_->bind();
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
                             {0,   -1, 0},
                             {1,  0,  0}}, {0, 1, 2});
    }

    void beginDraw() override {
        buffer_->bind();
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_;
};

} // namespace Graphic
#endif //FAKE3DRENDERER_TRIANGLE_HH
