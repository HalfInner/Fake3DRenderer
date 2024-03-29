//
// Created by kajbr on 22.02.2020.
//

#ifndef FAKE3DRENDERER_SHADERENGINE_HH
#define FAKE3DRENDERER_SHADERENGINE_HH

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <memory>

#include "Shader.hh"

class ShaderEngine {
  public:
    void addVertexShader(std::shared_ptr<ShaderSource> shader) {
        vertexShader_ = std::move(shader);
    }

    void addFragmentShader(std::shared_ptr<ShaderSource> shader) {
        fragmentShader_ = std::move(shader);
    }

    void initialize() {
        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        createVertexShader();
        createFragmentShader();

        initializeShaders();

        queueDeleteVertexShader();
        queueDeleteFragmentShader();

        isShadersInitialized_ = true;
    }

    void activate() {
        if (!isShadersInitialized_) {
            auto msg = "ERROR::SHADER::PROGRAM::NOT_INITIALIZED";
            std::cerr << msg << std::endl;
            throw std::runtime_error(msg);
        }

        glUseProgram(shaderProgram_);
    }

    void setColor(const glm::vec3 &color) {
        const std::string colorObjectName = "colorObject";
        setVec3(colorObjectName, color);
    }

    void setLightColor(const glm::vec3 &color) {
        const std::string lightColorObjectName = "lightColorObject";
        setVec3(lightColorObjectName, color);
    }

    void setLightPos(const glm::vec3 &color) {
        const std::string lightPosObjectName = "lightPosObject";
        setVec3(lightPosObjectName, color);
    }

    void setLightIntensity(const float intensity) {
        const std::string lightPosObjectName = "lightIntensityObject";
        setFloat(lightPosObjectName, intensity);
    }

    void setViewPos(const glm::vec3 &color) {
        const std::string viewPosObjectName = "viewPosObject";
        setVec3(viewPosObjectName, color);
    }

    void setProjection(const glm::mat4 &mat) const {
        const std::string projectionName = "projection";
        setMat4(projectionName, mat);
    }

    void setView(const glm::mat4 &mat) const {
        const std::string viewName = "view";
        setMat4(viewName, mat);
    }

    void setModel(const glm::mat4 &mat) const {
        const std::string modelName = "model";
        setMat4(modelName, mat);
    }

    void setAmbientRatio(float ambientRatio) const {
        const std::string ambientRatioName = "ambientRatio";
        setFloat(ambientRatioName, ambientRatio);
    }

    void setDiffuseRatio(float diffuseRatio) const {
        const std::string diffuseRatioName = "diffuseRatio";
        setFloat(diffuseRatioName, diffuseRatio);
    }

    void setSpecularRatio(float specularRatio) const {
        const std::string specularRatioName = "specularRatio";
        setFloat(specularRatioName, specularRatio);
    }

    void setFocusRatio(float focusRatio) const {
        const std::string focusRatioName = "focusRatio";
        setFloat(focusRatioName, focusRatio);
    }

    void setTextureOn(bool isTextureOn) const {
        const std::string textureOnName = "textureOn";
        setInteger(textureOnName, static_cast<int>(isTextureOn));
    }
    
  private:
    void setInteger(const std::string &name, int value) const {
        auto varId = glGetUniformLocation(shaderProgram_, name.c_str());
        glUniform1i(varId, value);
    }
    void setFloat(const std::string &name, float value) const {
        auto varId = glGetUniformLocation(shaderProgram_, name.c_str());
        glUniform1f(varId, value);
    }

    void setVec3(const std::string &name, const glm::vec3 &vec) const {
        auto varId = glGetUniformLocation(shaderProgram_, name.c_str());
        glUniform3fv(varId, 1, &vec[0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        auto varId = glGetUniformLocation(shaderProgram_, name.c_str());
        glUniformMatrix4fv(varId, 1, GL_FALSE, &mat[0][0]);
    }

    void initializeShaders() {
        // link shaders
        shaderProgram_ = glCreateProgram();
        glAttachShader(shaderProgram_, vertexShaderId_);
        glAttachShader(shaderProgram_, fragmentShaderId_);
        glLinkProgram(shaderProgram_);
        // check for linking errors
        int success = 0;
        glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram_, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }

    void createFragmentShader() {// fragment shader
        fragmentShaderId_ = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderId_, 1, fragmentShader_->shader(), nullptr);
        glCompileShader(fragmentShaderId_);
        // check for shader compile errors
        int success = 0;
        glGetShaderiv(fragmentShaderId_, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShaderId_, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    void createVertexShader() {
        vertexShaderId_ = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderId_, 1, vertexShader_->shader(), nullptr);
        glCompileShader(vertexShaderId_);
        // check for shader compile errors
        int success = 0;
        glGetShaderiv(vertexShaderId_, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShaderId_, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    void queueDeleteFragmentShader() const { glDeleteShader(fragmentShaderId_); }

    void queueDeleteVertexShader() const { glDeleteShader(vertexShaderId_); }

    int shaderProgram_;
    std::shared_ptr<ShaderSource> vertexShader_;
    std::shared_ptr<ShaderSource> fragmentShader_;
    GLuint vertexShaderId_;
    GLuint fragmentShaderId_;
    bool isShadersInitialized_ = false;
};

#endif //FAKE3DRENDERER_SHADERENGINE_HH
