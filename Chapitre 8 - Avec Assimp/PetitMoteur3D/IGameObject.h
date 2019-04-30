#pragma once
#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
  class IGameObject
  {
  public:
    using id_type = unsigned long;
    virtual id_type GetId() const = 0;
    virtual void OnSpawn(const physx::PxTransform& pose) = 0;
    virtual void OnUnspawn() = 0;
    virtual ~IGameObject() = default;
  };
}