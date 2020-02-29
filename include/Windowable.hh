//
// Created by kajbr on 23.02.2020.
//

#ifndef FAKE3DRENDERER_WINDOWABLE_HH
#define FAKE3DRENDERER_WINDOWABLE_HH

#include <chrono>
#include <iostream>
#include <glm/vec4.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <thread>

#include "InputController.hh"
#include "ShaderEngine.hh"
#include "Shader.hh"
#include "ShaderManager.hh"
#include "Renderable.hh"
#include "Triangle.hh"

struct Windowable {
    virtual void initialize() = 0;
    virtual void run() = 0;
    virtual ~Windowable() = default;
};

class SimpleClock {
  public:
    void resume() {
        begin_ = std::chrono::steady_clock::now();
    }

    auto measure() {
        end_ = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_ - begin_).count();
        begin_ = end_;

        return elapsed;
    }

  private:
    std::chrono::steady_clock::time_point begin_{};
    std::chrono::steady_clock::time_point end_{};
};

class SimpleWindow : public Windowable {
  public:
    SimpleWindow() :
            shaderManagerPtr_(std::make_shared<ShaderManager>()),
            vs_(std::make_shared<VertexShaderSource>(shaderManagerPtr_)),
            fs_(std::make_shared<FragmentShaderSource>(shaderManagerPtr_)) {
        shaderEngine_ = std::make_shared<ShaderEngine>();
    }

    void initialize() override {
        initializeGlfw();
        createWindowContext();

        initializeGlew();

        configureInputController();

        shaderEngine_->addFragmentShader(fs_);
        shaderEngine_->addVertexShader(vs_);
        shaderEngine_->initialize();
        shaderEngine_->activate();

        camera_ = std::make_shared<MovableCamera>();

        tppCamera_ = std::make_unique<Graphic::BasicRenderer>(shaderEngine_, camera_);
//        tppCamera_->addObject(std::make_shared<Graphic::Triangle>());
//        tppCamera_->addObject(std::make_shared<Graphic::TriangleInv>());
        tppCamera_->addObject(std::make_shared<Graphic::Cube>());
        tppCamera_->initialize();
    }

    void run() override {
        SimpleClock sc;
        sc.resume();
        while (!glfwWindowShouldClose(window_)) {
            auto elapsed = sc.measure();
            openGlInputController_->serve();

            if (auto wiseCamera = std::dynamic_pointer_cast<MovableCamera>(camera_)) {
                glm::vec3 leftDirectionVector {1, 0, 0};
                leftDirectionVector *= elapsed;
                wiseCamera->move(leftDirectionVector);
            }

            tppCamera_->draw(elapsed);

            glfwSwapBuffers(window_);
            glfwPollEvents();

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
        }
    }

    ~SimpleWindow() override {
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
        openGlInputController_->subscribeEnterPress([wd = window_]() {
            glfwSetWindowShouldClose(wd, true);
        });
        openGlInputController_->subscribeEscapePress([wd = window_]() {
            std::cout << "ESCAPE CLICKED\n";
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
        glfwSetFramebufferSizeCallback(window_, [](auto win, auto width, auto height) {
            // make sure the viewport matches the new window_ dimensions; note that width and
            // height will be significantly larger than specified on retina displays.
            glViewport(0, 0, width, height);
        });
    }

    void initializeGlfw() const {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    GLFWwindow *window_ = nullptr;

    ShaderManagerPtr shaderManagerPtr_ = nullptr;
    std::shared_ptr<VertexShaderSource> vs_ = nullptr;
    std::shared_ptr<FragmentShaderSource> fs_ = nullptr;
    std::shared_ptr<ShaderEngine> shaderEngine_ = nullptr;

    std::unique_ptr<Graphic::BasicRenderer> tppCamera_ = nullptr;

    std::unique_ptr<OpenGlInputController> openGlInputController_ = nullptr;

    int initialScreenWidth_ = 0;
    int initialScreenHeight_ = 0;
    std::shared_ptr<MovableCamera> camera_;
};

#endif //FAKE3DRENDERER_WINDOWABLE_HH
