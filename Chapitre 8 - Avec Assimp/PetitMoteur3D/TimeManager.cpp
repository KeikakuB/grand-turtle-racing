#include "stdafx.h"
#include "TimeManager.h"
#include <iostream>

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(TimeManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<TimeManager>() noexcept { return{ GetInterfaceId<ITimeManager>() }; }
  }

  TimeManager::TimeManager()
    : last_frame_time_()
    , current_time_()
    , session_timer_()
    , frame_delay_()
  {
    session_timer_.reset();
    startSession();
    startFrame();
  }

  void TimeManager::RunTask()
  {
    startFrame();
  }

  TimeManager::time_point TimeManager::currentFrameTime() const
  {
    return current_time_;
  }

  TimeManager::time_point TimeManager::lastFrameTime() const
  {
    return last_frame_time_;
  }

  void TimeManager::setGameRate(float iRate)
  {
    session_timer_.setRate(iRate);
  }

  void TimeManager::startFrame()
  {
    last_frame_time_ = current_time_;
    current_time_ = session_timer_.now();
    real_last_frame_time_ = real_current_time_;
    real_current_time_ = real_timer_.now();
  }

  void TimeManager::startSession()
  {
    using namespace std::chrono_literals;
    //TODO(Bill): need to undefine min macro here for below code to work -> where does min macro come from -> stop it
#undef min
#undef max
    frame_delay_ = std::chrono::milliseconds(1000 / 60);
    current_time_ = TimeManager::time_point::min();
    last_frame_time_ = TimeManager::time_point::min();
    real_current_time_ = TimeManager::time_point::min();
    real_last_frame_time_ = TimeManager::time_point::min();
    real_timer_.reset(1.0f);
    session_timer_.reset(1.0f);
  }

  TimeManager::time_point TimeManager::nowGameTime() const
  {
    return session_timer_.now();
  }

  bool TimeManager::shouldShowNextFrame() const
  {
    return real_timer_.now() - real_last_frame_time_ > frame_delay_;
  }

  float TimeManager::getGameRate() const
  {
    return session_timer_.rate();
  }

  TimeManager::time_point TimeManager::nowWorldTime() const
  {
    return real_timer_.now();
  }
}