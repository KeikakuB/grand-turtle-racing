#pragma once
#include "Meta.h"
#include "ITimeManager.h"

#include <string>

namespace PM3D
{
  class IPostEffectManager
  {
  public:
    virtual void SetDefaultTechnique(const std::string& name) = 0;
    virtual void EnqueueTechnique(const std::string& name, ITimeManager::duration time_to_show, ITimeManager::duration delay_at_end) = 0;
    virtual void StartPostEffect() = 0;
    virtual void EndPostEffect() = 0;
    virtual void DrawPostEffect() = 0;
    virtual ~IPostEffectManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IPostEffectManager);
  }
}