//
// Created by kajbr on 21.02.2020.
//

#ifndef FAKE3DRENDERER_RENDERABLE_H
#define FAKE3DRENDERER_RENDERABLE_H

#include <glm/vec4.hpp>
#include <memory>
#include <vector>

#include "ShaderEngine.hh"

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


struct /*interface*/ Renderable {
    virtual std::vector<float> trianglazation() = 0;
    virtual ~Renderable() = default;
};

struct /*interface*/ Camera {
    virtual Result draw() = 0;
    virtual Result addObject(std::shared_ptr<Renderable> renderable) = 0;
    virtual Result setPosition(glm::vec4 position) = 0;
    virtual Result initialize() = 0;
    virtual ~Camera() = default;
};

class TPPCamera : public Camera {
  public:
    explicit TPPCamera(std::shared_ptr<ShaderEngine> shaderEngine) : shaderEngine_(std::move(shaderEngine)) {}

    Result addObject(std::shared_ptr<Renderable> renderable) override {
        objects_.emplace_back(std::move(renderable));
        return Result::Success;
    }

    Result setPosition(glm::vec4 position) override {
        position_ = position;
        return Result::Success;
    }

    Result draw() override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        // glUseProgram(shaderProgram_);
        glUseProgram(shaderEngine_->handler());
        // seeing as we only have a single VAO_ there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO_);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        // glBindVertexArray(0); // no need to unbind it every time

        return Result::Success;
    }

    Result initialize() override {
        // generated from object
        float vertices[] = {
                0.5f, 0.5f, 0.0f,  // top right
                0.5f, -0.5f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f,  // bottom left
                -0.5f, 0.5f, 0.0f   // top left
        };

        unsigned int indices[] = {  // note that we start from 0!
                0, 1, 3,  // first Triangle
//                1, 2, 3,   // second Triangle
                0, 1, 2   // second Triangle
        };

        glGenVertexArrays(1, &VAO_);
        glGenBuffers(1, &VBO_);
        glGenBuffers(1, &EBO_);

        glBindVertexArray(VAO_);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO_ as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO_ while a VAO_ is active as the bound element buffer object IS stored in the VAO_; keep the EBO_ bound.
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO_ afterwards so other VAO_ calls won't accidentally modify this VAO_, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        // uncomment this call to draw in wireframe polygons.
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        return Result::Success;
    }

    ~TPPCamera() override {
        glDeleteVertexArrays(1, &VAO_);
        glDeleteBuffers(1, &VBO_);
        glDeleteBuffers(1, &EBO_);
    }

  private:
    unsigned int VAO_ = 0;
    unsigned int VBO_ = 0;
    unsigned int EBO_ = 0;
    std::shared_ptr<ShaderEngine> shaderEngine_;
    std::vector<std::shared_ptr<Renderable>> objects_;
    glm::vec4 position_;
};

} // namespace Graphic
#endif //FAKE3DRENDERER_RENDERABLE_H
