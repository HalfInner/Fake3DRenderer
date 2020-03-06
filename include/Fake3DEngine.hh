//
// Created by kajbr on 23.02.2020.
//

#ifndef FAKE3DRENDERER_FAKE3DENGINE_HH
#define FAKE3DRENDERER_FAKE3DENGINE_HH

#include <chrono>
#include <iostream>
#include <glm/vec4.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <thread>

#include "InputController.hh"
#include "Shader.hh"
#include "ShaderEngine.hh"
#include "ShaderEngineFactory.hh"
#include "ShaderManager.hh"
#include "Renderable.hh"
#include "Triangle.hh"

struct Fake3DEngine {
    virtual void initialize() = 0;
    virtual void run() = 0;
    virtual ~Fake3DEngine() = default;
};

class SimpleClock {
  public:
    void resume() {
        begin_ = std::chrono::high_resolution_clock::now();
    }

    auto measure() {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = end - begin_;
        begin_ = end;

        return std::chrono::duration<double>(elapsed).count();
    }

  private:
    std::chrono::high_resolution_clock::time_point begin_{};
};

class BasicFake3DEngine : public Fake3DEngine {
  public:
    BasicFake3DEngine() {
    }

    void initialize() override {
        initializeGlfw();
        createWindowContext();

        initializeGlew();

        camera_ = std::make_shared<MovableCamera>();
        camera_->updateScreen(initialScreenWidth_, initialScreenHeight_);

        prepareScene();

        configureInputController();
    }

    void prepareScene() {
        basicRenderer_ = std::make_unique<Graphic::BasicRenderer>(camera_);

        // Ground
        basicRenderer_->addObject(std::make_shared<Graphic::Surface>());

        // Flat of building
        basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(glm::vec3(-2, 0, 0)));
        basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
                glm::vec3{-2.f, 1.f, -1.f}, glm::vec3{1.f,3.f, 1.f}));
        basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
                glm::vec3{-2.f, 1.f, -1.f}, glm::vec3{1.f,3.f, 1.f}));
        basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
                glm::vec3{2.f, 1.f, -4.f}, glm::vec3{1.f,3.f, 3.f}));
        basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
                glm::vec3{2.f, .5f, -8.f}, glm::vec3{1.f,2.f, 4.f}));
        basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
                glm::vec3{-2.f, .5f, -7.f}, glm::vec3{1.f,2.f, 4.f}));

        // Road
        basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
                glm::vec3{0.f, -0.45f, 0.f}, glm::vec3{1.f,0.01f, 200.f}, glm::vec3{.2f, .2f, .2f}));


        // Pacman
        basicRenderer_->addObject(std::make_shared<Graphic::NaiveSphere>(0.75f, glm::vec3{-3.f, 0.f, -4.f}));

        // Snowman
        basicRenderer_->addObject(std::make_shared<Graphic::NaiveSphere>(0.5f, glm::vec3{1.f, 0.f, -2.f}));
        basicRenderer_->addObject(std::make_shared<Graphic::NaiveSphere>(0.25f, glm::vec3{1.f, .7f, -2.f}));
        basicRenderer_->addObject(std::make_shared<Graphic::NaiveSphere>(0.1f, glm::vec3{1.f, 1.f, -2.f}));


        // Light
        auto sun = std::make_shared<Graphic::SunSphere>();
        basicRenderer_->addObject(sun);
        basicRenderer_->addLight(sun);

        basicRenderer_->initialize();
    }

    void run() override {
        SimpleClock sc;
        sc.resume();
        while (!glfwWindowShouldClose(window_)) {
            auto elapsed = sc.measure();
            std::cout << "TEST " << elapsed << "\n";
            openGlInputController_->serve(elapsed);
            basicRenderer_->draw(elapsed);

            glfwSwapBuffers(window_);
            glfwPollEvents();

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
        }
    }

    ~BasicFake3DEngine() override {
        glfwTerminate();
    }

  private:
    void initializeGlew() const {
        auto err = glewInit();
        if (err != GLEW_OK) {
            constexpr auto errorMsg = "Failed to initialize GLEW library";
            std::cerr << errorMsg;
            glfwTerminate();
            throw std::runtime_error(errorMsg);
        }

        if (!GLEW_VERSION_2_1) { // check that the machine supports the 2.1 API.
            // or handle the error in a nicer way
            constexpr auto errorMsg = "GLEW does not support the latest needed features";
            std::cerr << errorMsg;
            glfwTerminate();
            throw std::runtime_error(errorMsg);
        }
    }

    void configureInputController() {
        openGlInputController_ = std::make_unique<OpenGlInputController>(window_);
        openGlInputController_->subscribeEnterPress([wd = window_](void *param) {
            glfwSetWindowShouldClose(wd, true);
        });
        openGlInputController_->subscribeEscapePress([wd = window_](void *param) {
            glfwSetWindowShouldClose(wd, true);
        });

        std::weak_ptr<MovableCamera> weakCamera(camera_);
        openGlInputController_->subscribeWPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->move(Movable::Direction::Forward, elapsed);
            }
        });
        openGlInputController_->subscribeSPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->move(Movable::Direction::Backward, elapsed);
            }
        });
        openGlInputController_->subscribeAPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->move(Movable::Direction::Leftward, elapsed);
            }
        });
        openGlInputController_->subscribeDPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->move(Movable::Direction::Rightward, elapsed);
            }
        });
        openGlInputController_->subscribeQPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->move(Movable::Direction::Upward, elapsed);
            }
        });
        openGlInputController_->subscribeEPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->move(Movable::Direction::Downward, elapsed);
            }
        });

        openGlInputController_->subscribeZPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->zoom(Zoomable::ResizeType::ZoomIn, elapsed);
            }
        });
        openGlInputController_->subscribeCPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->zoom(Zoomable::ResizeType::ZoomOut, elapsed);
            }
        });

        openGlInputController_->subscribeRPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->rotate(Rotatable::HeadDirection::LeftSide, elapsed);
            }
        });
        openGlInputController_->subscribeTPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->rotate(Rotatable::HeadDirection::RightSide, elapsed);
            }
        });
        openGlInputController_->subscribeFPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->rotate(Rotatable::HeadDirection::DownSide, elapsed);
            }
        });
        openGlInputController_->subscribeGPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->rotate(Rotatable::HeadDirection::UpSide, elapsed);
            }
        });
        openGlInputController_->subscribeVPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->rotate(Rotatable::HeadDirection::LeftShoulder, elapsed);
            }
        });
        openGlInputController_->subscribeBPress([weakCamera](void *param) {
            float elapsed = *reinterpret_cast<float *>(param);
            if (auto camera = weakCamera.lock()) {
                camera->rotate(Rotatable::HeadDirection::RightShoulder, elapsed);
            }
        });
    }

    void createWindowContext() {
        // glfw window_ creation
        // --------------------
        initialScreenWidth_ = 800;
        initialScreenHeight_ = 640;
        window_ = glfwCreateWindow(initialScreenWidth_, initialScreenHeight_, "LearnOpenGL", nullptr, nullptr);
        if (window_ == nullptr) {
            constexpr auto errorMsg = "Failed to create GLFW window";
            std::cerr << errorMsg;
            glfwTerminate();
            throw std::runtime_error(errorMsg);
        }

        glfwMakeContextCurrent(window_);

        glfwSetWindowUserPointer(window_, this);
        auto updateScreenCB = [](GLFWwindow * window, int width, int height) {
            glViewport(0, 0, width, height);
            static_cast<BasicFake3DEngine*>(glfwGetWindowUserPointer(window))->camera_->updateScreen(width, height);
        };
        glfwSetFramebufferSizeCallback(window_, updateScreenCB);
    }

    void initializeGlfw() const {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void updateScreenCB(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
        camera_->updateScreen(width, height);
    }

    GLFWwindow *window_ = nullptr;

    ShaderManagerPtr shaderManagerPtr_ = nullptr;
    std::shared_ptr<VertexShaderSource> vs_ = nullptr;
    std::shared_ptr<FragmentShaderSource> fs_ = nullptr;

    std::unique_ptr<Graphic::BasicRenderer> basicRenderer_ = nullptr;

    std::unique_ptr<OpenGlInputController> openGlInputController_ = nullptr;

    int initialScreenWidth_ = 0;
    int initialScreenHeight_ = 0;
    std::shared_ptr<MovableCamera> camera_;
};

#endif //FAKE3DRENDERER_FAKE3DENGINE_HH
