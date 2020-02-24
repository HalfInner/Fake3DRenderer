//
// Created by kajbr on 21.02.2020.
//

#ifndef FAKE3DRENDERER_RENDERABLE_H
#define FAKE3DRENDERER_RENDERABLE_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

#include "ShaderEngine.hh"
#include "Triangle.hh"

namespace Graphic {

enum class Result {
    Success,
    Fail,
    NotApplicable
};

struct /*interface*/ Movable {
    virtual Result move(glm::vec4 vec) = 0;
    virtual ~Movable() = default;
};


struct /*interface*/ Camera {
    virtual Result initialize() = 0;
    virtual Result draw() = 0;
    virtual Result addObject(std::shared_ptr<Renderable> renderable) = 0;
    virtual Result setPosition(glm::vec4 position) = 0;
    virtual ~Camera() = default;
};

class TPPCamera : public Camera {
  public:
    explicit TPPCamera(std::shared_ptr<ShaderEngine> shaderEngine) : shaderEngine_(std::move(shaderEngine)) {}

    Result initialize() override {
        // generated from object
        for (auto &&object : objects_) {
            object->initialize(std::make_shared<OpenGlBuffer>());
        }

        return Result::Success;
    }


    Result draw() override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        // glUseProgram(shaderProgram_);
        glUseProgram(shaderEngine_->handler());
        // seeing as we only have a single VAO_ there's no need to bind it every time, but we'll do so to keep things a bit more organized

        for (auto &&object : objects_) {
            object->beginDraw();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            object->endDraw();
        }

        return Result::Success;
    }

    Result addObject(std::shared_ptr<Renderable> renderable) override {
        objects_.emplace_back(std::move(renderable));
        return Result::Success;
    }

    Result setPosition(glm::vec4 position) override {
        position_ = position;
        return Result::Success;
    }

    ~TPPCamera() override {
    }

  private:

    void prepareObjects() {
//        auto cube = std::make_shared<Graphic::Triangle>();
//        addObject(cube);
        // uncomment this call to draw in wireframe polygons.
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    static constexpr unsigned DELETE_BUFFER_KEY = 0;

    unsigned int VAO_ = 0;
    unsigned int VBO_ = 0;
    unsigned int EBO_ = 0;
    std::shared_ptr<ShaderEngine> shaderEngine_;
    std::vector<std::shared_ptr<Renderable>> objects_;
    glm::vec4 position_;

    std::vector<glm::vec3> vertices{};
    std::vector<unsigned> indices{};
};

} // namespace Graphic
#endif //FAKE3DRENDERER_RENDERABLE_H
