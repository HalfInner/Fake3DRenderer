//#include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>

//#include <GL/gl.h>
//#include <GL/glu.h>

#include <iostream>
#include "source/Renderable.hh"
#include "source/ShaderManager.hh"
#include "source/Shader.hh"
#include "source/ShaderEngine.hh"
#include "source/InputController.hh"
#include "source/Windowable.hh"

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
