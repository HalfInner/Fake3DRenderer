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
#include "ShaderEngineFactory.hh"
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
    virtual Result addLight(std::shared_ptr<LightPoint> light) = 0;
    virtual ~Renderer() = default;
};

class BasicRenderer : public Renderer {
  public:
    explicit BasicRenderer(std::shared_ptr<Camera> camera)
            : camera_(std::move(camera)) {}

    Result initialize() override {
        initializeShaders();

        for (auto &&object : objects_) {
            object->initialize(std::make_shared<OpenGlBuffer>());
        }

        return Result::Success;
    }

    Result draw(float elapsedTime) override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto &light = lights_.front(); // TODO(kaj): Supports only one light at the moment
        (void) light;
        for (auto &&object : objects_) {
            auto info = object->beginDraw(elapsedTime);

            auto &shaderEngine = shaders.at(info.typeObject);
            shaderEngine->activate();

            shaderEngine->setProjection(camera_->projection());
            shaderEngine->setView(camera_->view());

            shaderEngine->setColor(info.color);
            shaderEngine->setLightColor(light->color());
            shaderEngine->setLightPos(light->position());
            shaderEngine->setViewPos(camera_->position());

            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, info.position);
            float angle = 1.f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.f, 1.f));
            shaderEngine->setModel(model);

            if (info.debugMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            glDrawElements(info.renderType, info.elements, info.countType, nullptr);
            object->endDraw();

            while (auto errorCode = glGetError()) {
                std::cerr << "OpenGL troubles: 0x" << std::hex << errorCode << "\n";
                throw std::runtime_error("OpenGL problem. Not Allowed at the moment");
            }
        }

        return Result::Success;
    }

    Result addObject(std::shared_ptr<Renderable> renderable) override {
        objects_.emplace_back(std::move(renderable));
        return Result::Success;
    }

    Result addLight(std::shared_ptr<LightPoint> light) override {
        lights_.emplace_back(std::move(light));
        return Result::Success;
    }

  private:
    void initializeShaders() {
        ShaderEngingeFactory factory;
        auto normalShaderEngine = factory.create(RendererInfo::TypeObject::Normal);
        auto lightShaderEngine = factory.create(RendererInfo::TypeObject::Light);
        auto notSpecifiedShaderEngine = factory.create(RendererInfo::TypeObject::NotSpecified);
        // generated from object

        shaders[RendererInfo::TypeObject::Normal] = std::move(normalShaderEngine);
        shaders[RendererInfo::TypeObject::Light] = std::move(lightShaderEngine);
        shaders[RendererInfo::TypeObject::NotSpecified] = std::move(notSpecifiedShaderEngine);
    }

    std::unordered_map<Graphic::RendererInfo::TypeObject, std::unique_ptr<ShaderEngine>> shaders;

    std::shared_ptr<Camera> camera_{nullptr};

    std::vector<std::shared_ptr<Renderable>> objects_{};
    std::vector<std::shared_ptr<LightPoint>> lights_{};

};

} // namespace Graphic
#endif //FAKE3DRENDERER_RENDERABLE_H
