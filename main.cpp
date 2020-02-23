#include "Renderable.hh"
#include "InputController.hh"
#include "Windowable.hh"

namespace Graphic {

class Triangle : public Renderable, public Movable {
  public:
    std::vector<float> trianglazation() override {
        return std::vector<float>();
    }

    Result move(glm::vec4 vec) override {
        pos_ += vec;
        return Result::Success;
    }

  private:
    glm::vec4 pos_{};
};

} // namespace Graphic

int main() {
    SimpleWindow sw;
    sw.initialize();
    sw.run();
}
