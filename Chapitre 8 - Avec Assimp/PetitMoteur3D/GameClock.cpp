#include "stdafx.h"
#include "GameClock.h"

using namespace std;
using namespace std::chrono;

namespace PM3D
{
  static GameClock::native_time_point GetTimeStamp()
  {
    return high_resolution_clock::now();
  }

  static GameClock::duration GetElapsedTime(GameClock::native_time_point start_time, GameClock::native_time_point end_time)
  {
    return end_time - start_time;
  }

  void GameClock::reset(float rate)
  {
    acc_time_ = 0ms;
    start_time_ = GetTimeStamp();
    rate_ = rate;
  }

  void GameClock::setRate(float rate)
  {
    native_time_point now_time = GetTimeStamp();

    acc_time_ += GetElapsedTime(start_time_, now_time) * rate_;
    start_time_ = now_time;
    rate_ = rate;
  }

  GameClock::time_point GameClock::now() const noexcept
  {
    native_time_point now_time = GetTimeStamp();
    return time_point(acc_time_ + GetElapsedTime(start_time_, now_time) * rate_);
  }
}