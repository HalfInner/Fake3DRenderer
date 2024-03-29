//
// Created by kajbr on 08.03.2020.
//

#include "Fake3DEngine.hh"

#include <iostream>
#include <thread>

#include "Cuboid.hh"
#include "InputController.hh"
#include "GraphicBase.hh"
#include "NaiveSphere.hh"
#include "PoolBall.hh"
#include "SimpleClock.hh"
#include "SunSphere.hh"
#include "Surface.hh"
#include "TextureCuboid.hh"

void BasicFake3DEngine::initialize() {
    initializeGlfw();
    createWindowContext();

    initializeGlew();

    camera_ = std::make_shared<MovableCamera>();
    camera_->updateScreen(static_cast<float>(initialScreenWidth_), static_cast<float>(initialScreenHeight_));

    prepareScene();

    configureInputController();
}

void BasicFake3DEngine::prepareScene() {
    basicRenderer_ = std::make_unique<Graphic::BasicRenderer>(camera_);

    // Ground
    basicRenderer_->addObject(std::make_shared<Graphic::Surface>());

    // Flat of building
    float rgbRatio = 1. / 255.f;
    basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(glm::vec3(-2, 0, 0)));
    basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
            glm::vec3{-2.f, 1.f, -1.f}, glm::vec3{1.f, 3.f, 1.f}, glm::vec3{178, 132, 62} * rgbRatio));
    basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
            glm::vec3{-2.f, 1.f, -1.f}, glm::vec3{1.f, 3.f, 1.f}, glm::vec3{178, 116, 62} * rgbRatio));
    basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
            glm::vec3{2.f, 1.f, -4.f}, glm::vec3{1.f, 3.f, 3.f}, glm::vec3{252, 156, 12} * rgbRatio));
    basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
            glm::vec3{2.f, .5f, -8.f}, glm::vec3{1.f, 2.f, 4.f}, glm::vec3{252, 124, 12} * rgbRatio));
    basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
            glm::vec3{-2.f, .5f, -7.f}, glm::vec3{1.f, 2.f, 4.f}, glm::vec3{204, 126, 10} * rgbRatio));

    // Road
    basicRenderer_->addObject(std::make_shared<Graphic::Cuboid>(
            glm::vec3{0.f, -0.45f, 0.f}, glm::vec3{1.f, 0.01f, 200.f}, glm::vec3{.2f, .2f, .2f}));

    // Pacman
    auto pacManColor = glm::vec3{0.99f, 0.99f, -.99f};
    basicRenderer_->addObject(
            std::make_shared<Graphic::NaiveSphere>(0.75f, glm::vec3{-3.f, 0.f, -4.f}, pacManColor));

    // Snowman
    auto snowManColor = glm::vec3{0.99f, 0.99f, .99f};
    basicRenderer_->addObject(
            std::make_shared<Graphic::NaiveSphere>(0.5f, glm::vec3{1.f, 0.f, -2.f}, snowManColor));
    basicRenderer_->addObject(
            std::make_shared<Graphic::NaiveSphere>(0.25f, glm::vec3{1.f, .7f, -2.f}, snowManColor));
    basicRenderer_->addObject(
            std::make_shared<Graphic::NaiveSphere>(0.1f, glm::vec3{1.f, 1.f, -2.f}, snowManColor));


    // Pool Ball - Phong
    basicRenderer_->addObject(std::make_shared<Graphic::PoolBall>(.7f, glm::vec3{2, 0, 1.5f}));
    basicRenderer_->addObject(
            std::make_shared<Graphic::PoolBall>(.7f, glm::vec3{0, 0, 1.5f}, glm::vec3{0.5f, 0.5f, 0.78f}));
    // Cuboid Wall - Phong
    basicRenderer_->addObject(std::make_shared<Graphic::TextureCuboid>(
            glm::vec3{-2.f, 1.f, 1.5f}, glm::vec3{1.f, 3.f, 1.f}, glm::vec3{178, 132, 62} * rgbRatio));


    // Light
    sun_ = std::make_shared<Graphic::SunSphere>();
    basicRenderer_->addObject(sun_);
    basicRenderer_->addLight(sun_);

    // Material
    material_ = std::make_shared<Utils::GlobalMaterial>();
    basicRenderer_->setMaterial(material_);

    basicRenderer_->initialize();
}

void BasicFake3DEngine::run() {
    SimpleClock sc;
    sc.resume();
    while (!glfwWindowShouldClose(window_)) {
        auto elapsed = sc.measure();
        openGlInputController_->serve(elapsed);
        basicRenderer_->draw(elapsed);

        glfwSwapBuffers(window_);
        glfwPollEvents();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10ms);
    }
}

BasicFake3DEngine::~BasicFake3DEngine() {
    glfwTerminate();
}

void BasicFake3DEngine::initializeGlew() {
    auto err = gladLoadGL();
    if (err == 0) {
        constexpr auto errorMsg = "Failed to initialize GLEW library";
        std::cerr << errorMsg;
        glfwTerminate();
        throw std::runtime_error(errorMsg);
    }

    // if (!GLEW_VERSION_2_1) { // check that the machine supports the 2.1 API.
    //     // or handle the error in a nicer way
    //     constexpr auto errorMsg = "GLEW does not support the latest needed features";
    //     std::cerr << errorMsg;
    //     glfwTerminate();
    //     throw std::runtime_error(errorMsg);
    // }
}

void BasicFake3DEngine::configureInputController() {
    openGlInputController_ = std::make_unique<OpenGlInputController>(window_);
    openGlInputController_->subscribeEnterPress([sun = sun_](void *param) {
        static bool isSunRunning = false;
        static float lastMeasure = INFINITY;
        float elapsed = *reinterpret_cast<float *>(param);
        if (lastMeasure - elapsed < 0.15f) {
            lastMeasure += elapsed;

            return;
        }
        lastMeasure = .0f;
        isSunRunning ? sun->stop() : sun->start();
        isSunRunning = !isSunRunning;
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
            camera->rotate(Rotatable::HeadDirection::UpSide, elapsed);
        }
    });
    openGlInputController_->subscribeGPress([weakCamera](void *param) {
        float elapsed = *reinterpret_cast<float *>(param);
        if (auto camera = weakCamera.lock()) {
            camera->rotate(Rotatable::HeadDirection::DownSide, elapsed);
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

    auto weakSun = std::weak_ptr<Graphic::SunSphere>(sun_);
    openGlInputController_->subscribeMinusPress([weakSun](void *param) {
        float elapsed = *reinterpret_cast<float *>(param);
        if (auto sun = weakSun.lock()) {
            sun->decreaseIntensity(elapsed);
        }
    });

    openGlInputController_->subscribeEqualPress([weakSun](void *param) {
        float elapsed = *reinterpret_cast<float *>(param);
        if (auto sun = weakSun.lock()) {
            sun->increaseIntensity(elapsed);
        }
    });

    auto weakMaterial = std::weak_ptr<Utils::GlobalMaterial>(material_);
    openGlInputController_->subscribeLeftBracketPress([weakMaterial](void *param) {
        float elapsed = *reinterpret_cast<float *>(param);
        if (auto material = weakMaterial.lock()) {
            material->decreaseAmbient(elapsed);
        }
    });

    openGlInputController_->subscribeRightBracketPress([weakMaterial](void *param) {
        float elapsed = *reinterpret_cast<float *>(param);
        if (auto material = weakMaterial.lock()) {
            material->increaseAmbient(elapsed);
        }
    });

    openGlInputController_->subscribeColonPress([weakMaterial](void *param) {
        float elapsed = *reinterpret_cast<float *>(param);
        if (auto material = weakMaterial.lock()) {
            material->decreaseDiffuse(elapsed);
        }
    });

    openGlInputController_->subscribeApostrophePress([weakMaterial](void *param) {
        float elapsed = *reinterpret_cast<float *>(param);
        if (auto material = weakMaterial.lock()) {
            material->increaseDiffuse(elapsed);
        }
    });

    openGlInputController_->subscribeDotPress([weakMaterial](void *param) {
        float elapsed = *reinterpret_cast<float *>(param);
        if (auto material = weakMaterial.lock()) {
            material->decreaseSpecular(elapsed);
        }
    });

    openGlInputController_->subscribeSlashPress([weakMaterial](void *param) {
        float elapsed = *reinterpret_cast<float *>(param);
        if (auto material = weakMaterial.lock()) {
            material->increaseSpecular(elapsed);
        }
    });

    openGlInputController_->subscribeMPress([weakMaterial](void *param) {
        float elapsed = *reinterpret_cast<float *>(param);
        if (auto material = weakMaterial.lock()) {
            material->decreaseFocus(elapsed);
        }
    });

    openGlInputController_->subscribeCommaPress([weakMaterial](void *param) {
        float elapsed = *reinterpret_cast<float *>(param);
        if (auto material = weakMaterial.lock()) {
            material->increaseFocus(elapsed);
        }
    });

    openGlInputController_->subscribeSpacePress([weakMaterial](void *param) {
        static float lastMeasure = INFINITY;
        float elapsed = *reinterpret_cast<float *>(param);
        if (lastMeasure - elapsed < 0.15f) {
            lastMeasure += elapsed;
            return;
        }
        lastMeasure = .0f;

        if (auto material = weakMaterial.lock()) {
            material->toggleTexture();
        }
    });
}

void BasicFake3DEngine::createWindowContext() {
    // glfw window_ creation
    // --------------------
    initialScreenWidth_ = 1280;
    initialScreenHeight_ = 800;
    const std::string windowName = "Projekt GKIW - Kajetan Brzuszczak - 301023 OKNO";
    window_ = glfwCreateWindow(initialScreenWidth_, initialScreenHeight_, windowName.c_str(), nullptr, nullptr);
    if (window_ == nullptr) {
        constexpr auto errorMsg = "Failed to create GLFW window";
        std::cerr << errorMsg;
        glfwTerminate();
        throw std::runtime_error(errorMsg);
    }

    glfwMakeContextCurrent(window_);

    glfwSetWindowUserPointer(window_, this);
    auto updateScreenCB = [](GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
        static_cast<BasicFake3DEngine *>(glfwGetWindowUserPointer(window))->camera_->updateScreen(
                static_cast<float>(width), static_cast<float>(height));
    };
    glfwSetFramebufferSizeCallback(window_, updateScreenCB);
}

void BasicFake3DEngine::initializeGlfw() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}