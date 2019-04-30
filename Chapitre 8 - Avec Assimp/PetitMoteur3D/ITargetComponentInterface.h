#pragma once
#include "Meta.h"
#include "GameObject.h"

namespace PM3D
{
  class ITargetComponentInterface
  {
  public:
    virtual GameObject* GetTarget() = 0;
    virtual void SetTarget(GameObject::id_type t) = 0;
    virtual ~ITargetComponentInterface() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(ITargetComponentInterface);
  }
}