//#include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>

//#include <GL/gl.h>
//#include <GL/glu.h>

#include <iostream>
#include "Renderable.hh"
#include "ShaderManager.hh"
#include "Shader.hh"
#include "ShaderEngine.hh"
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
