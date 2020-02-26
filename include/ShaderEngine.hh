//
// Created by kajbr on 22.02.2020.
//

#ifndef FAKE3DRENDERER_SHADERENGINE_HH
#define FAKE3DRENDERER_SHADERENGINE_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>
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

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        auto varId = glGetUniformLocation(shaderProgram_, name.c_str());
        glUniformMatrix4fv(varId, 1, GL_FALSE, &mat[0][0]);
    }


  private:
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
