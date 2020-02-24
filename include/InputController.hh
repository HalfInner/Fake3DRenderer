//
// Created by kajbr on 22.02.2020.
//

#ifndef FAKE3DRENDERER_INPUTCONTROLLER_HH
#define FAKE3DRENDERER_INPUTCONTROLLER_HH

#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <iostream>

#include "Shader.hh"
#include "ShaderManager.hh"
#include "Renderable.hh"
#include "InputController.hh"

struct /* interface */ InputController {
    using InputControllerCB = std::function<void()>;
    virtual void subscribeEnterPress(InputControllerCB cb) {};
    virtual void subscribeEscapePress(InputControllerCB cb) {};

    virtual void serve() = 0;

    virtual ~InputController() = default;
};

class OpenGlInputController : public InputController {
  public:
    explicit OpenGlInputController(GLFWwindow *window) : window_(window) {}

    void serve() override {
        if (glfwGetKey(window_, GLFW_KEY_ENTER) == GLFW_PRESS) {
            if (cbEnterPress_) cbEnterPress_();
        }
        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            if (cbEscapePress_) cbEscapePress_();
        }
    }

    void subscribeEscapePress(InputControllerCB cb) override {
        cbEscapePress_ = cb;
    }

    void subscribeEnterPress(InputControllerCB cb) override {
        cbEnterPress_ = cb;
    }

  private:
    InputControllerCB cbEnterPress_;
    InputControllerCB cbEscapePress_;
    GLFWwindow *window_;
};
#endif //FAKE3DRENDERER_INPUTCONTROLLER_HH
