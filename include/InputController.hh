//
// Created by kajbr on 22.02.2020.
//

#ifndef FAKE3DRENDERER_INPUTCONTROLLER_HH
#define FAKE3DRENDERER_INPUTCONTROLLER_HH

#include <functional>
#include <GLFW/glfw3.h>

struct /* interface */ InputController {
    using InputControllerCB = std::function<void(void *)>;

    virtual void subscribeEnterPress(InputControllerCB &&cb) {};

    virtual void subscribeEscapePress(InputControllerCB &&cb) {};

    // Move
    virtual void subscribeWPress(InputControllerCB &&cb) {};

    virtual void subscribeSPress(InputControllerCB &&cb) {};

    virtual void subscribeAPress(InputControllerCB &&cb) {};

    virtual void subscribeDPress(InputControllerCB &&cb) {};

    virtual void subscribeQPress(InputControllerCB &&cb) {};

    virtual void subscribeEPress(InputControllerCB &&cb) {};

    // rotate
    virtual void subscribeRPress(InputControllerCB &&cb) {};

    virtual void subscribeTPress(InputControllerCB &&cb) {};

    virtual void subscribeFPress(InputControllerCB &&cb) {};

    virtual void subscribeGPress(InputControllerCB &&cb) {};

    virtual void subscribeVPress(InputControllerCB &&cb) {};

    virtual void subscribeBPress(InputControllerCB &&cb) {};

    // Zoom
    virtual void subscribeZPress(InputControllerCB &&cb) {};

    virtual void subscribeCPress(InputControllerCB &&cb) {};

    virtual void serve(float elapsedTime) = 0;


    // Light manipulation
    virtual void subscribeMinusPress(InputControllerCB &&cb) = 0;
    virtual void subscribeEqualPress(InputControllerCB &&cb) = 0;

    // Ambient
    virtual void subscribeLeftBracketPress(InputControllerCB &&cb) = 0;
    virtual void subscribeRightBracketPress(InputControllerCB &&cb) = 0;

    // diffuse
    virtual void subscribeColonPress(InputControllerCB &&cb) = 0;
    virtual void subscribeApostrophePress(InputControllerCB &&cb) = 0;

    // specular
    virtual void subscribeDotPress(InputControllerCB &&cb) = 0;
    virtual void subscribeSlashPress(InputControllerCB &&cb) = 0;

    // focus
    virtual void subscribeMPress(InputControllerCB &&cb) = 0;
    virtual void subscribeCommaPress(InputControllerCB &&cb) = 0;

    // focus
    virtual void subscribeSpacePress(InputControllerCB &&cb) = 0;

    virtual ~InputController() = default;
};

class OpenGlInputController : public InputController {
  public:
    explicit OpenGlInputController(GLFWwindow *window) : window_(window) {}

    void serve(float elapsedTime) override {
        if (glfwGetKey(window_, GLFW_KEY_ENTER) == GLFW_PRESS) {
            if (cbEnterPress_) cbEnterPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            if (cbEscapePress_) cbEscapePress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
            if (cbWPress_) cbWPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
            if (cbSPress_) cbSPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
            if (cbAPress_) cbAPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
            if (cbDPress_) cbDPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
            if (cbQPress_) cbQPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
            if (cbEPress_) cbEPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_R) == GLFW_PRESS) {
            if (cbRPress_) cbRPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_T) == GLFW_PRESS) {
            if (cbTPress_) cbTPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_F) == GLFW_PRESS) {
            if (cbFPress_) cbFPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_G) == GLFW_PRESS) {
            if (cbGPress_) cbGPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_V) == GLFW_PRESS) {
            if (cbVPress_) cbVPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_B) == GLFW_PRESS) {
            if (cbBPress_) cbBPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS) {
            if (cbZPress_) cbZPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS) {
            if (cbCPress_) cbCPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_MINUS) == GLFW_PRESS) {
            if (cbMinusPress_) cbMinusPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_EQUAL) == GLFW_PRESS) {
            if (cbEqualPress_) cbEqualPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
            if (cbLBracketPress_) cbLBracketPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
            if (cbRBracketPress_) cbRBracketPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
            if (cbColonPress_) cbColonPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_APOSTROPHE) == GLFW_PRESS) {
            if (cbApostrophePress_) cbApostrophePress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_PERIOD) == GLFW_PRESS) {
            if (cbDotPress_) cbDotPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_SLASH) == GLFW_PRESS) {
            if (cbSlashPress_) cbSlashPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_M) == GLFW_PRESS) {
            if (cbMPress_) cbMPress_(&elapsedTime);
        }
        if (glfwGetKey(window_, GLFW_KEY_COMMA) == GLFW_PRESS) {
            if (cbCommaPress_) cbCommaPress_(&elapsedTime);
        }
    }

    void subscribeEscapePress(InputControllerCB &&cb) override {
        cbEscapePress_ = cb;
    }

    void subscribeEnterPress(InputControllerCB &&cb) override {
        cbEnterPress_ = cb;
    }

    void subscribeWPress(InputControllerCB &&cb) override {
        cbWPress_ = cb;
    }

    void subscribeSPress(InputControllerCB &&cb) override {
        cbSPress_ = cb;
    }

    void subscribeAPress(InputControllerCB &&cb) override {
        cbAPress_ = cb;
    }

    void subscribeDPress(InputControllerCB &&cb) override {
        cbDPress_ = cb;
    }

    void subscribeQPress(InputControllerCB &&cb) override {
        cbQPress_ = cb;
    }

    void subscribeEPress(InputControllerCB &&cb) override {
        cbEPress_ = cb;
    }

    void subscribeRPress(InputControllerCB &&cb) override {
        cbRPress_ = cb;
    }

    void subscribeTPress(InputControllerCB &&cb) override {
        cbTPress_ = cb;
    }

    void subscribeFPress(InputControllerCB &&cb) override {
        cbFPress_ = cb;
    }

    void subscribeGPress(InputControllerCB &&cb) override {
        cbGPress_ = cb;
    }

    void subscribeVPress(InputControllerCB &&cb) override {
        cbVPress_ = cb;
    }

    void subscribeBPress(InputControllerCB &&cb) override {
        cbBPress_ = cb;
    }

    void subscribeZPress(InputControllerCB &&cb) override {
        cbZPress_ = cb;
    }

    void subscribeCPress(InputControllerCB &&cb) override {
        cbCPress_ = cb;
    }

    void subscribeMinusPress(InputControllerCB &&cb) override {
        cbMinusPress_ = cb;
    }

    void subscribeEqualPress(InputControllerCB &&cb) override {
        cbEqualPress_ = cb;
    }

    void subscribeLeftBracketPress(InputControllerCB &&cb) override {
        cbLBracketPress_ = cb;
    }

    void subscribeRightBracketPress(InputControllerCB &&cb) override {
        cbRBracketPress_ = cb;
    }

    void subscribeColonPress(InputControllerCB &&cb) override {
        cbColonPress_ = cb;
    }

    void subscribeApostrophePress(InputControllerCB &&cb) override {
        cbApostrophePress_ = cb;
    }

    void subscribeDotPress(InputControllerCB &&cb) override {
        cbDotPress_ = cb;
    }

    void subscribeSlashPress(InputControllerCB &&cb) override {
        cbSlashPress_ = cb;
    }

    void subscribeMPress(InputControllerCB &&cb) override {
        cbMPress_ = cb;
    }

    void subscribeCommaPress(InputControllerCB &&cb) override {
        cbCommaPress_ = cb;
    }

    void subscribeSpacePress(InputControllerCB &&cb) override {
        cbSpacePress_ = cb;
    }

  private:
    InputControllerCB cbEnterPress_;
    InputControllerCB cbEscapePress_;
    InputControllerCB cbWPress_;
    InputControllerCB cbSPress_;
    InputControllerCB cbAPress_;
    InputControllerCB cbDPress_;
    InputControllerCB cbQPress_;
    InputControllerCB cbEPress_;
    InputControllerCB cbRPress_;
    InputControllerCB cbTPress_;
    InputControllerCB cbFPress_;
    InputControllerCB cbGPress_;
    InputControllerCB cbVPress_;
    InputControllerCB cbBPress_;
    InputControllerCB cbCPress_;
    InputControllerCB cbZPress_;
    InputControllerCB cbMinusPress_;
    InputControllerCB cbEqualPress_;
    InputControllerCB cbLBracketPress_;
    InputControllerCB cbRBracketPress_;
    InputControllerCB cbColonPress_;
    InputControllerCB cbApostrophePress_;
    InputControllerCB cbDotPress_;
    InputControllerCB cbSlashPress_;
    InputControllerCB cbMPress_;
    InputControllerCB cbCommaPress_;
    InputControllerCB cbSpacePress_;

    GLFWwindow *window_;
};
#endif //FAKE3DRENDERER_INPUTCONTROLLER_HH
