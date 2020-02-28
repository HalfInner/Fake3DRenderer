//
// Created by kajbr on 28.02.2020.
//

#ifndef FAKE3DRENDERER_BASICCAMERA_HH
#define FAKE3DRENDERER_BASICCAMERA_HH

#include <glm/matrix.hpp>
#include <glm/ext.hpp>

struct /*interface*/ Camera {
    virtual glm::mat4 projection() = 0;
    virtual glm::mat4 view() = 0;
    virtual glm::mat4 model() = 0;
};

class BasicCamera : public Camera {
  public:
    glm::mat4 projection() override;
    glm::mat4 view() override;
    glm::mat4 model() override;
};


#endif //FAKE3DRENDERER_BASICCAMERA_HH
