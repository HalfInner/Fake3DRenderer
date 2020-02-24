//
// Created by kajbr on 23.02.2020.
//

#ifndef FAKE3DRENDERER_TRIANGLE_HH
#define FAKE3DRENDERER_TRIANGLE_HH

class Triangle : public Graphic::Renderable, public Graphic::Movable {
  public:
    std::vector<glm::vec3> trianglazation() override {
        return {{-1, 0, 0},
                {1,  0, 0},
                {0,  1, 0}};
    }

    Graphic::Result move(glm::vec4 vec) override {
        pos_ += vec;
        return Graphic::Result::Success;
    }

  private:
    glm::vec4 pos_{};
};

#endif //FAKE3DRENDERER_TRIANGLE_HH
