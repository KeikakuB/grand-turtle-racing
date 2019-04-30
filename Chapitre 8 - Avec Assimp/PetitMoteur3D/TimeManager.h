#pragma once
#include "Meta.h"
#include "GameClock.h"
#include "GameManagerComponent.h"
#include "ITimeManager.h"
namespace PM3D
{
  class TimeManager final :
    public GameManagerComponent, public ITimeManager
  {
  public:
    using time_point = GameClock::time_point;
    using duration = GameClock::duration;
    TimeManager();
    virtual void RunTask() override;
    virtual time_point currentFrameTime() const override;
    virtual time_point lastFrameTime() const override;
    virtual void setGameRate(float iRate) override;
    virtual float getGameRate() const override;
    virtual void startFrame() override;
    virtual void startSession() override;
    virtual time_point nowGameTime() const override;
    virtual time_point nowWorldTime() const override;
    virtual bool shouldShowNextFrame() const override;

  private:
    time_point last_frame_time_;
    time_point current_time_;
    GameClock session_timer_;
    time_point real_last_frame_time_;
    time_point real_current_time_;
    duration frame_delay_;
    GameClock real_timer_;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(TimeManager);
  }
}