#include "Renderable.hh"
#include "InputController.hh"
#include "Windowable.hh"

namespace Graphic {

class Triangle : public Renderable {
  public:
    Result draw() override {
        return Result::Fail;
    }
};

} // namespace Graphic

int main() {
    SimpleWindow sw;
    sw.initialize();
    sw.run();
}
