//
// Created by kajbr on 23.02.2020.
//

#ifndef FAKE3DRENDERER_FAKE3DENGINE_HH
#define FAKE3DRENDERER_FAKE3DENGINE_HH

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "BasicCamera.hh"
#include "InputController.hh"
#include "Renderable.hh"
#include "GraphicBase.hh"
#include "Utils.hh"
#include "SunSphere.hh"

struct Fake3DEngine {
    virtual void initialize() = 0;
    virtual void run() = 0;
    virtual ~Fake3DEngine() = default;
};

class BasicFake3DEngine : public Fake3DEngine {
  public:
    void initialize() override;
    void prepareScene();

    void run() override;

    ~BasicFake3DEngine() override;

  private:
    void initializeGlew();
    void configureInputController();

    void createWindowContext();
    void initializeGlfw();
    GLFWwindow *window_ = nullptr;

    std::unique_ptr<Graphic::BasicRenderer> basicRenderer_ = nullptr;
    std::unique_ptr<OpenGlInputController> openGlInputController_ = nullptr;

    int initialScreenWidth_ = 0;
    int initialScreenHeight_ = 0;
    std::shared_ptr<MovableCamera> camera_;
    std::shared_ptr<Graphic::SunSphere> sun_;
    std::shared_ptr<Utils::GlobalMaterial> material_;
};

#endif //FAKE3DRENDERER_FAKE3DENGINE_HH
