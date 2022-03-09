//
// Created by kajbr on 22.02.2020.
//

#ifndef FAKE3DRENDERER_SHADERMANAGER_HH
#define FAKE3DRENDERER_SHADERMANAGER_HH

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Utils.hh"

class ShaderManager;
using ShaderManagerPtr = std::shared_ptr<ShaderManager>;

class ShaderManager {
  public:
    ShaderManager() :
            shaders(SHADERS_LIMITS, nullptr), shaderCounter(0) {
    }

    ~ShaderManager() = default;

    uint32_t addShader(const char *shaderSource) {
        if (auto hasReachedLimit = shaderCounter == SHADERS_LIMITS; hasReachedLimit) {
            throw std::runtime_error("Cannot create new shader. Limit has been reached");
        }
        ++shaderCounter;
        auto id = shaders.size();
        shaders.emplace_back(shaderSource);
        return id;
    }

    const char **shader(uint32_t id) {
        if (auto isInRange = id < shaders.size(); !isInRange) {
            throw std::runtime_error("Out of scope");
        }
        return shaders.data() + id;
    }
  private:
    static constexpr size_t SHADERS_LIMITS = 10;

    std::vector<const char *> shaders;
    uint32_t shaderCounter = 0;
};

#endif //FAKE3DRENDERER_SHADERMANAGER_HH
