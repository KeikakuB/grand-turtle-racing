#pragma once
#include "Meta.h"
#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
  class IPoseComponentInterface
  {
  public:
    virtual physx::PxTransform GetTransform() const = 0;
    virtual void SetTransform(const physx::PxTransform& t) = 0;
    virtual ~IPoseComponentInterface() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IPoseComponentInterface);
  }
}