//
// Created by kajbr on 24.02.2020.
//

#ifndef FAKE3DRENDERER_BUFFER_HH
#define FAKE3DRENDERER_BUFFER_HH

#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <vector>

namespace Graphic {

enum class BufferType {
    Regular, Normal, NormalWithTexture
};

struct /*interface*/ Buffer {
    virtual void initialize(
            std::vector<glm::vec3> &&points, std::vector<unsigned> &&indices, BufferType bufferType) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual ~Buffer() = default;
};

class OpenGlBuffer : public Buffer {
  public:
    void initialize(std::vector<glm::vec3> &&points, std::vector<unsigned> &&indices, BufferType bufferType) override {
        glGenVertexArrays(1, &VAO_);
        glGenBuffers(1, &VBO_);
        glGenBuffers(1, &EBO_);

        bind();
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

        switch (bufferType) {
            case BufferType::Regular:
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
                glEnableVertexAttribArray(0);
                break;
            case BufferType::Normal:
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void *) (sizeof(glm::vec3)));
                glEnableVertexAttribArray(1);
                break;
            case BufferType::NormalWithTexture:
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 9 * sizeof(float), (void *) (sizeof(glm::vec3)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 9 * sizeof(float),
                                      (void *) (sizeof(glm::vec3) + sizeof(glm::vec3)));
                glEnableVertexAttribArray(2);
                break;
        }

        unbind();
    }

    void bind() override {
        glBindVertexArray(VAO_);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    }

    void unbind() override {
        glBindBuffer(GL_ARRAY_BUFFER, DELETE_BUFFER_KEY);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DELETE_BUFFER_KEY);
        glBindVertexArray(DELETE_BUFFER_KEY);
    }

  private:
    static constexpr unsigned DELETE_BUFFER_KEY = 0;

    unsigned int VAO_ = 0;
    unsigned int VBO_ = 0;
    unsigned int EBO_ = 0;
};

} // namespace Graphic

#endif //FAKE3DRENDERER_BUFFER_HH
