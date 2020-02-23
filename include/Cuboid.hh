//
// Created by kajbr on 21.02.2020.
//

#ifndef FAKE3DRENDERER_CUBOID_H
#define FAKE3DRENDERER_CUBOID_H

#include "Renderable.hh"

#include <stdexcept>

namespace Graphic {

class Cuboid : public Renderable {
  public:
    Cuboid() {}


    // Interface Renderable
    Result draw() override {
        throw std::runtime_error("Not Implemented");
//        return Result::Success;
    }

  private:
};

} // namespace Graphic

#endif //FAKE3DRENDERER_CUBOID_H
