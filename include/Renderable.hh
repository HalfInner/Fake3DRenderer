//
// Created by kajbr on 21.02.2020.
//

#ifndef FAKE3DRENDERER_RENDERABLE_H
#define FAKE3DRENDERER_RENDERABLE_H

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

#include "BasicCamera.hh"
#include "ShaderEngine.hh"
#include "Triangle.hh"

namespace Graphic {

enum class Result {
    Success,
    Fail,
    NotApplicable
};

struct /*interface*/ Renderer {
    virtual Result initialize() = 0;
    virtual Result draw(float elapsedTime) = 0;
    virtual Result addObject(std::shared_ptr<Renderable> renderable) = 0;
    virtual ~Renderer() = default;
};


class BasicRenderer : public Renderer {
  public:
    explicit BasicRenderer(std::shared_ptr<ShaderEngine> shaderEngine, std::shared_ptr<Camera> camera)
            : shaderEngine_(std::move(shaderEngine)), camera_(std::move(camera)) {}

    Result initialize() override {

        // generated from object
        for (auto &&object : objects_) {
            object->initialize(std::make_shared<OpenGlBuffer>());
        }

        return Result::Success;
    }

    Result draw(float elapsedTime) override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderEngine_->setProjection(camera_->projection());
        shaderEngine_->setView(camera_->view());

        for (auto &&object : objects_) {
            auto info = object->beginDraw(elapsedTime);
            shaderEngine_->setColor(info.color);

            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, info.position);
            float angle = 1.f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.f, 1.f));
            shaderEngine_->setModel(model);


            if (info.debugMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            glDrawElements(info.renderType, info.elements, info.countType, nullptr);
            object->endDraw();
        }

        return Result::Success;
    }

    Result addObject(std::shared_ptr<Renderable> renderable) override {
        objects_.emplace_back(std::move(renderable));
        return Result::Success;
    }

  private:

    unsigned int VAO_ = 0;
    unsigned int VBO_ = 0;
    unsigned int EBO_ = 0;
    std::shared_ptr<ShaderEngine> shaderEngine_{nullptr};
    std::shared_ptr<Camera> camera_{nullptr};

    std::vector<std::shared_ptr<Renderable>> objects_;

    glm::vec3 cameraPos{glm::vec3(0.0f, 0.0f, 3.0f)};
    glm::vec3 cameraTarget{glm::vec3(0.0f, 0.0f, 0.0f)};
    glm::vec3 cameraDirection{};
    glm::mat4 view_{};

    std::vector<glm::vec3> vertices{};
    std::vector<unsigned> indices{};
};

} // namespace Graphic
#endif //FAKE3DRENDERER_RENDERABLE_H
