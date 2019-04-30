#pragma once

#include "GameClockBase.h"

#include <chrono>

namespace PM3D
{
  class GameClock : public GameClockBase
  {
  public:
    using native_time_point = std::chrono::high_resolution_clock::time_point;

    static native_time_point native_now() noexcept
    {
      return std::chrono::high_resolution_clock::now();
    }

  public:
    time_point now() const noexcept;

  public:
    void reset(float rate = 1.0f);
    void setRate(float rate);
    float rate() const { return rate_; }

  private:
    native_time_point start_time_ = GameClock::native_now();
    duration acc_time_ = duration(0);
    float rate_ = 1.0f;
  };
} // namespace engine
