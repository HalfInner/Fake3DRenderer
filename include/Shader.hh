//
// Created by kajbr on 22.02.2020.
//

#ifndef FAKE3DRENDERER_SHADER_HH
#define FAKE3DRENDERER_SHADER_HH

#include "ShaderManager.hh"

struct ShaderSource {
    virtual const char **shader() = 0;
    virtual ~ShaderSource() = default;
};

class VertexShaderSource : public ShaderSource {
  public:
    explicit VertexShaderSource(ShaderManagerPtr shaderManager) :
            shaderManager_(std::move(shaderManager)) {
        id_ = shaderManager_->addShader(shaderProgram);
    }

    const char **shader() override {
        return shaderManager_->shader(id_);
    }

  private:
    static constexpr const char *shaderProgram = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord;

            out vec2 TexCoord;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            void main() {
                gl_Position = projection * view * model * vec4(aPos, 1.0f);
                TexCoord = vec2(aTexCoord.x, aTexCoord.y);
            })";
//    gl_Position = projection * view * model * vec4(aPos, 1.0);


    uint32_t id_;
    ShaderManagerPtr shaderManager_;
};

class FragmentShaderSource : public ShaderSource {
  public:
    explicit FragmentShaderSource(ShaderManagerPtr shaderManager) :
            shaderManager_(std::move(shaderManager)) {
        id_ = shaderManager_->addShader(shaderProgram);
    }

    const char **shader() override {
        return shaderManager_->shader(id_);
    }

  private:
    static constexpr const char *shaderProgram = R"(
            #version 330 core
            out vec4 FragColor;
            void main() {
              FragColor = vec4(1.0f, 0.5f, 1.f, 0.8f);
            })";
    uint32_t id_;
    ShaderManagerPtr shaderManager_;
};

#endif //FAKE3DRENDERER_SHADER_HH
