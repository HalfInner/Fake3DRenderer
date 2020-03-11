//
// Created by kajbr on 23.02.2020.
//

#ifndef FAKE3DRENDERER_GRAPHICBASE_HH
#define FAKE3DRENDERER_GRAPHICBASE_HH

#include <cmath>

#include "Utils.hh"
#include "Buffer.hh"
namespace Graphic {

using Utils::TypeObject;
struct /*interface*/ RendererInfo {

    enum RenderType : unsigned {
        Triangles = GL_TRIANGLES,
        Lines = GL_LINES,
        UnknownRenderType = 0x0
    };

    enum CountType : unsigned {
        Byte = GL_UNSIGNED_BYTE,
        Short = GL_UNSIGNED_SHORT,
        Integer = GL_UNSIGNED_INT,
        UnknownCountType = 0x0
    };

    struct Material {
        // http://devernay.free.fr/cours/opengl/materials.html
        // Default is chrome
        glm::vec3 ambient {0.25f, 0.25f, 0.25f};
        glm::vec3 diffuse {0.4f, 0.4f, 0.4f};
        glm::vec3 specular {0.774597f,0.774597f,0.774597f};
        float shininess = 0.6f;
    };

    TypeObject typeObject = TypeObject::NotSpecified;
    RenderType renderType = RenderType::Triangles;
    CountType countType = CountType::Integer;
    unsigned elements = 0u;

    Material material {};

    glm::vec3 position{};
    glm::mat4 model {};
    glm::vec3 color{};


    bool debugMode = false;
};

struct /*interface*/ Renderable {
    virtual void initialize(std::shared_ptr<Buffer> buffer) = 0;
    virtual RendererInfo beginDraw(float elapsedTime) = 0;
    virtual void endDraw() = 0;
    virtual ~Renderable() = default;
};

struct /*interface*/ LightPoint {
    virtual glm::vec3 position() = 0;
    virtual glm::vec3 color() = 0;
    virtual float intensity() = 0;

    virtual void increaseIntensity(float elapsedTime) =0;
    virtual void decreaseIntensity(float elapsedTime) =0;
    virtual ~LightPoint() = default;
};

struct /*interface*/ Animation {
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ~Animation() = default;
};

} // namespace Graphic
#endif //FAKE3DRENDERER_GRAPHICBASE_HH
