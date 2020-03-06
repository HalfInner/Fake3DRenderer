//
// Created by kajbr on 05.03.2020.
//

#ifndef FAKE3DRENDERER_SHADERENGINEFACTORY_HH
#define FAKE3DRENDERER_SHADERENGINEFACTORY_HH

#include "Triangle.hh"

class ShaderEngingeFactory {
  public:
    ShaderEngingeFactory() :
            shaderManagerPtr_(std::make_shared<ShaderManager>()) {
    }

    std::unique_ptr<ShaderEngine> create(Utils::TypeObject type) {
        auto shaderEngine = std::make_unique<ShaderEngine>();

        std::shared_ptr<ShaderSource> vs;
        std::shared_ptr<ShaderSource> fs;
        switch (type) {
            case Utils::TypeObject::NotSpecified:
            case Utils::TypeObject::Normal:
                vs = std::make_shared<VertexShaderSource>(shaderManagerPtr_);
                fs = std::make_shared<FragmentShaderSource>(shaderManagerPtr_);
                break;
            case Utils::TypeObject::Light:
                vs = std::make_shared<LightVertexShaderSource>(shaderManagerPtr_);
                fs = std::make_shared<LightFragmentShaderSource>(shaderManagerPtr_);
                break;
            case Utils::TypeObject::PoolBall:
                vs = std::make_shared<PoolBallVertexShaderSource>(shaderManagerPtr_);
                fs = std::make_shared<PoolBallFragmentShaderSource>(shaderManagerPtr_);
                break;
            default:
                throw std::runtime_error("Unknown type");
        }
        shaderEngine->addFragmentShader(fs);
        shaderEngine->addVertexShader(vs);
        shaderEngine->initialize();

        return shaderEngine;
    }

  private:
    std::shared_ptr<ShaderManager> shaderManagerPtr_;
};

#endif //FAKE3DRENDERER_SHADERENGINEFACTORY_HH
