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

struct /*interface*/ Renderer {
    virtual Result initialize() = 0;
    virtual Result draw() = 0;
    virtual Result addObject(std::shared_ptr<Renderable> renderable) = 0;
    virtual Result setPosition(glm::vec4 position) = 0;
    virtual ~Renderer() = default;
};


class BasicRenderer : public Renderer {
  public:
    explicit BasicRenderer(std::shared_ptr<ShaderEngine> shaderEngine) : shaderEngine_(std::move(shaderEngine)) {}

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
//
//        const float radius = 10.0f;
//        float camX = sin(glfwGetTime()) * radius;
//        float camZ = cos(glfwGetTime()) * radius;
//
//        view_ = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//        int modelLoc = glGetUniformLocation(shaderEngine_->handler(), "model");
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


        // pass projection matrix to shader (note that in this case it could change every frame)
        auto cameraZoom = 45.f;
        glm::mat4 projection = glm::perspective(glm::radians(cameraZoom), (float)800 / (float)640, 0.1f, 100.0f);
        shaderEngine_->setMat4("projection", projection);

        // camera/view transformation
        auto position = glm::vec3(0.0f, 0.0f, 0.0f);
        auto up = glm::vec3(0.0f, 1.0f, 0.0f);
        auto front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::mat4 view = glm::lookAt(position, position + front, up);
        shaderEngine_->setMat4("view", view);


        for (auto &&object : objects_) {
            auto info = object->beginDraw();
            glDrawElements(info.type, info.elements, info.countType, nullptr);
            object->endDraw();
        }

        return Result::Success;
    }

    Result addObject(std::shared_ptr<Renderable> renderable) override {
        objects_.emplace_back(std::move(renderable));
        return Result::Success;
    }

    Result setPosition(glm::vec4 position) override {
//        position_ = position;
        return Result::Success;
    }

    ~BasicRenderer() override {
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

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection;
    glm::mat4 view_;

    std::vector<glm::vec3> vertices{};
    std::vector<unsigned> indices{};
};

} // namespace Graphic
#endif //FAKE3DRENDERER_RENDERABLE_H
