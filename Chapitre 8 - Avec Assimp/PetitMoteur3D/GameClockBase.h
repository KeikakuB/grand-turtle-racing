#pragma once
#include <chrono>

namespace PM3D
{
  struct GameClockBase
  {
    using duration = std::chrono::duration<double>;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<GameClockBase>;
    static const bool is_steady = true;
  };
}