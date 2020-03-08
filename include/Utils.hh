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
    float ambient;
    float diffuse;
    float specular;
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
