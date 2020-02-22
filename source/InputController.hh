//
// Created by kajbr on 22.02.2020.
//

#ifndef FAKE3DRENDERER_INPUTCONTROLLER_HH
#define FAKE3DRENDERER_INPUTCONTROLLER_HH


#include "Shader.hh"
#include "ShaderManager.hh"
#include "Renderable.hh"
#include <functional>
#include <iostream>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include "InputController.hh"

struct /* interface */ InputController {
    using InputControllerCB = std::function<void()>;
    virtual void subscribeEnterPress(InputControllerCB f) = 0;

    virtual void serve() = 0;

    virtual ~InputController() {}
};
class OpenGlInputController : public InputController {
  public:
    OpenGlInputController(GLFWwindow *window) : window_(window) {}

    void serve() override {
        if (glfwGetKey(window_, GLFW_KEY_ENTER) == GLFW_PRESS) {
            if (cb_) cb_();
        }
    }

    void subscribeEnterPress(InputControllerCB cb) override {
        cb_ = cb;
    }

  private:
    InputControllerCB cb_;
    GLFWwindow *window_;
};
#endif //FAKE3DRENDERER_INPUTCONTROLLER_HH
