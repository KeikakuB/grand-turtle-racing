#pragma once
#include "Meta.h"
#include "GameClockBase.h"
namespace PM3D
{
  class ITimeManager
  {
  public:
    using duration = GameClockBase::duration;
    using time_point = GameClockBase::time_point;
    virtual time_point nowGameTime() const = 0;
    virtual time_point nowWorldTime() const = 0;
    virtual time_point currentFrameTime() const = 0;
    virtual time_point lastFrameTime() const = 0;
    virtual bool shouldShowNextFrame() const = 0;
    virtual float getGameRate() const = 0;
    virtual void setGameRate(float iRate) = 0;
    virtual void startFrame() = 0;
    virtual void startSession() = 0;
    virtual ~ITimeManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(ITimeManager);
  }
}
