#pragma once
#include "ComponentObject.h"

namespace PM3D
{
  template <typename T>
  class IComponent
  {
  public:
    virtual void OnAttached(ComponentObject<T>& game_object) = 0;
    virtual void OnDetached(ComponentObject<T>& game_object) = 0;
    virtual ~IComponent() = default;
  };
}