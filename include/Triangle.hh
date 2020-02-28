//
// Created by kajbr on 23.02.2020.
//

#ifndef FAKE3DRENDERER_TRIANGLE_HH
#define FAKE3DRENDERER_TRIANGLE_HH

#include "Renderable.hh"
#include "Buffer.hh"

namespace Graphic {

enum RenderType : unsigned {
    Triangles = GL_TRIANGLES,
    UnknownRenderType = 0x0
};

enum CountType : unsigned {
    Byte = GL_UNSIGNED_BYTE,
    Short = GL_UNSIGNED_SHORT,
    Integer = GL_UNSIGNED_INT,
    UnknownCountType = 0x0
};

struct RendererInfo {
    RenderType type = RenderType::Triangles;
    unsigned elements = 0x0;
    CountType countType = CountType::Integer;
    glm::vec4 position {};
};

struct /*interface*/ Renderable {
    virtual void initialize(std::shared_ptr<Buffer> buffer) = 0;
    virtual RendererInfo beginDraw() = 0;
    virtual void endDraw() = 0;
    virtual ~Renderable() = default;
};

class Triangle : public Renderable {
  public:
    void initialize(std::shared_ptr<Buffer> buffer) override {
        buffer_ = std::move(buffer);

        buffer_->initialize({{-1, 0, 0},
                             {0,  1, 0},
                             {1,  0, 0}}, {0, 1, 2});
    }

    RendererInfo beginDraw() override {
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

    RendererInfo beginDraw() override {
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

class Cube : public Renderable {
  public:
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


        buffer_->initialize({front_t_1, front_t_2, front_t_3, front_t_4,
                             rear_t_1, rear_t_2, rear_t_3, rear_t_4,
                             left_t_1, left_t_2, left_t_3, left_t_4,
                             right_t_1, right_t_2, right_t_3, right_t_4},
                            {0, 1, 2, 1, 2, 3,
                             4, 5, 6, 5, 6, 7,
                             8, 9, 10, 9, 10, 11,
                             12, 13, 14, 13, 14, 15});
    }

    RendererInfo beginDraw() override {
        RendererInfo ri{};
        ri.elements = 16;

        buffer_->bind();

        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_;
};


} // namespace Graphic
#endif //FAKE3DRENDERER_TRIANGLE_HH
