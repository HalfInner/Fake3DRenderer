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
            layout (location = 1) in vec3 aNormal;

            out vec3 vertexColor;
            out vec3 lightColor;
            out vec3 lightPos;
            out vec3 viewPos;

            out vec3 FragPos;
            out vec3 Normal;

            uniform vec3 colorObject;
            uniform vec3 lightColorObject;
            uniform vec3 lightPosObject;
            uniform vec3 viewPosObject;

            uniform vec3 pos;
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            void main() {

                vertexColor = colorObject;
                lightColor = lightColorObject;
                lightPos = lightPosObject;
                viewPos = viewPosObject;

                FragPos = vec3(model * vec4(aPos, 1.0));
                Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
                gl_Position = projection * view * vec4(FragPos, 1.0);
            })";

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

            in vec3 Normal;
            in vec3 FragPos;

            in vec3 lightPos;
            in vec3 viewPos;
            in vec3 lightColor;
            in vec3 vertexColor;

            void main() {
                // ambient - works
                // float ambientStrength = 0.1;
                float ambientStrength = 0.1;
                vec3 ambient = ambientStrength * lightColor;

                // diffuse
                vec3 norm = normalize(Normal);
//                vec3 lightDir = normalize(lightPos - FragPos);
                vec3 lightDir = normalize(FragPos - lightPos * 10);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * lightColor;
                FragColor = vec4(diffuse, 1.0);

                // specular
                float specularStrength = 0.5;
                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
                vec3 specular = specularStrength * spec * lightColor;

                vec3 result = (ambient + diffuse + specular) * vertexColor;
                // result = vec3(1, 1,1);
//                FragColor = vec4(result, 1.0);
                FragColor = vec4(diffuse * vertexColor, 1.0);
            })";

    uint32_t id_;
    ShaderManagerPtr shaderManager_;
};

class LightVertexShaderSource : public ShaderSource {
  public:
    explicit LightVertexShaderSource(ShaderManagerPtr shaderManager) :
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

            uniform vec3 colorObject;

            uniform vec3 pos;
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            void main() {
                gl_Position = projection * view * model * vec4(aPos, 1.0f);
                TexCoord = vec2(aTexCoord.x, aTexCoord.y);
                vertexColor = colorObject;
            })";

    uint32_t id_;
    ShaderManagerPtr shaderManager_;
};

class LightFragmentShaderSource : public ShaderSource {
  public:
    explicit LightFragmentShaderSource(ShaderManagerPtr shaderManager) :
            shaderManager_(std::move(shaderManager)) {
        id_ = shaderManager_->addShader(shaderProgram);
    }

    const char **shader() override {
        return shaderManager_->shader(id_);
    }

  private:
    static constexpr const char *shaderProgram = R"(
            #version 330 core

            in vec3 vertexColor;
            out vec4 FragColor;
            void main() {
                FragColor = vec4(vertexColor, 1.0);
            })";

    uint32_t id_;
    ShaderManagerPtr shaderManager_;
};

#endif //FAKE3DRENDERER_SHADER_HH
