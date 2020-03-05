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
            out vec3 vertexColor;
            out vec3 lightColor;

            uniform vec3 colorObject;
            uniform vec3 lightColorObject;

            uniform vec3 pos;
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            void main() {
                gl_Position = projection * view * model * vec4(aPos, 1.0f);
                TexCoord = vec2(aTexCoord.x, aTexCoord.y);
                vertexColor = colorObject;
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

            in vec3 lightColor;
            in vec3 vertexColor;

            out vec4 FragColor;
            void main() {
                float ambientStrength = 0.1;
                vec3 ambient = ambientStrength * lightColor;

                vec3 result = ambient * objectColor;
                FragColor = vec4(result, 1.0);
            })";
    uint32_t id_;
    ShaderManagerPtr shaderManager_;
};

#endif //FAKE3DRENDERER_SHADER_HH
