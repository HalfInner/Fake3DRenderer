//
// Created by kajbr on 08.03.2020.
//

#ifndef FAKE3DRENDERER_SIMPLECLOCK_HH
#define FAKE3DRENDERER_SIMPLECLOCK_HH

#include <chrono>

class SimpleClock {
  public:
    void resume() {
        begin_ = std::chrono::high_resolution_clock::now();
    }

    auto measure() {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = end - begin_;
        begin_ = end;

        return std::chrono::duration<double>(elapsed).count();
    }

  private:
    std::chrono::high_resolution_clock::time_point begin_{};
};


#endif //FAKE3DRENDERER_SIMPLECLOCK_HH
