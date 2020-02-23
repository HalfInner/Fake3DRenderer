//
// Created by kajbr on 21.02.2020.
//

#ifndef FAKE3DRENDERER_RENDERABLE_H
#define FAKE3DRENDERER_RENDERABLE_H

namespace Graphic {

enum class Result {
    Success,
    Fail,
    NotApplicable
};

struct /*interface*/ Renderable {
    virtual Result draw() = 0;
    virtual ~Renderable() = default;
};

struct /*interface*/ Camera {
    virtual Result draw() = 0;
    virtual ~Camera() = default;
};

} // namespace Graphic
#endif //FAKE3DRENDERER_RENDERABLE_H
