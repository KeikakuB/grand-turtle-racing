#pragma once

#include "PxPhysicsAPI.forward.h"
namespace PM3D
{
  class ICollisionHandler
  {
  public:
    virtual void onContact(const physx::PxContactPair& contact_pair) = 0;
    virtual void onTrigger(bool trigger_enter, physx::PxShape* actor_shape, physx::PxShape* contact_shape) = 0;
    virtual ~ICollisionHandler() = default;
  };
}