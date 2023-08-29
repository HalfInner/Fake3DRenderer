
// Copyright HalfsInner 2023

#ifndef FAKE3DRENDERER_PLATFORMUTILS_HH
#define FAKE3DRENDERER_PLATFORMUTILS_HH

#include <filesystem>
#include <stdexcept>

namespace platform::utils {
  inline void initialize(int argc, char *argv[]) {
    if (argc == 0) {
      throw std::runtime_error("Currupt arguments");
    }
    auto p = std::filesystem::path(argv[0]);
    std::filesystem::current_path(p.parent_path());
  }
}

#endif  // FAKE3DRENDERER_PLATFORMUTILS_HH