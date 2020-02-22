//
// Created by kajbr on 22.02.2020.
//

#ifndef FAKE3DRENDERER_SHADER_HH
#define FAKE3DRENDERER_SHADER_HH

struct ShaderSource {
    virtual const char **shader() = 0;

    virtual ~ShaderSource() {};

};

class VertexShaderSource : public ShaderSource {
  public:
    VertexShaderSource(ShaderManagerPtr shaderManager) :
            shaderManager_(shaderManager) {
        id_ = shaderManager_->addShader(shaderProgram);
    }

    const char **shader() override {
        return shaderManager_->shader(id_);
    }

  private:
    static constexpr const char *shaderProgram = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            void main() {
                gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
            })";
    uint32_t id_;
    ShaderManagerPtr shaderManager_;
};

class FragmentShaderSource : public ShaderSource {
  public:
    FragmentShaderSource(ShaderManagerPtr shaderManager) :
            shaderManager_(shaderManager) {
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
              FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
            })";
    uint32_t id_;
    ShaderManagerPtr shaderManager_;
};

#endif //FAKE3DRENDERER_SHADER_HH
