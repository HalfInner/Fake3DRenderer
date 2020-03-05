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

    std::shared_ptr<ShaderEngine> create(Graphic::RendererInfo::TypeObject type) {
        auto shaderEngine = std::make_shared<ShaderEngine>();

        std::shared_ptr<VertexShaderSource> vs;
        std::shared_ptr<FragmentShaderSource> fs;
        switch (type) {
            case Graphic::RendererInfo::TypeObject::Normal:
                vs = std::make_shared<VertexShaderSource>(shaderManagerPtr_);
                fs = std::make_shared<FragmentShaderSource>(shaderManagerPtr_);
                break;
            case Graphic::RendererInfo::TypeObject::Light:
            case Graphic::RendererInfo::TypeObject::NotSpecified:
            default:
                throw std::runtime_error("Unknown type");
        }
        shaderEngine->addFragmentShader(fs);
        shaderEngine->addVertexShader(vs);

        return shaderEngine;
    }

  private:
    std::shared_ptr<ShaderManager> shaderManagerPtr_;
};

#endif //FAKE3DRENDERER_SHADERENGINEFACTORY_HH
