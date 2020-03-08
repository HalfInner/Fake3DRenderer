//
// Created by kajbr on 22.02.2020.
//

#ifndef FAKE3DRENDERER_UTILS_HH
#define FAKE3DRENDERER_UTILS_HH

namespace Utils {

enum class TypeObject {
    Normal, Light, PoolBall, NotSpecified
};

struct GlobalMaterial {
    float ambient{.1f};
    float diffuse{1.f};
    float specular{0.5f};
    float focus{5.f};

    void increaseAmbient(float elapsedTime) {
        ambient += elapsedTime * velocity;
        ambient = std::min(ambient, 2.f);
    }

    void increaseDiffuse(float elapsedTime) {
        diffuse += elapsedTime * velocity;
        diffuse = std::min(diffuse, 2.f);
    }

    void increaseSpecular(float elapsedTime) {
        specular += elapsedTime * velocity;
        specular = std::min(specular, 2.f);
    }

    void increaseFocus(float elapsedTime) {
        focus += elapsedTime * velocity * 10.f;
        focus = std::min(focus, 14.f);
    }

    void decreaseAmbient(float elapsedTime) {
        ambient -= elapsedTime * velocity;
        ambient = std::max(ambient, 0.f);
    }

    void decreaseDiffuse(float elapsedTime) {
        diffuse -= elapsedTime * velocity;
        diffuse = std::max(diffuse, 0.f);
    }

    void decreaseSpecular(float elapsedTime) {
        specular -= elapsedTime * velocity;
        specular = std::max(specular, 0.f);
    }

    void decreaseFocus(float elapsedTime) {
        focus -= elapsedTime * velocity * 10.f;
        focus = std::max(focus, 0.f);
    }

    static constexpr float velocity = 2.f;
};

struct NotCreatable {
    NotCreatable() = delete;
    ~NotCreatable() = delete;

    NotCreatable(const NotCreatable &) = delete;
    NotCreatable(NotCreatable &&) = delete;

    NotCreatable &operator=(const NotCreatable &) = delete;
    NotCreatable &operator=(NotCreatable &&) = delete;
};

} // namespace Utils
#endif //FAKE3DRENDERER_UTILS_HH
