#pragma once
#include "ICollisionHandler.h"

namespace PM3D
{
  class CollisionHandler :
    public ICollisionHandler
  {
  public:
    void onContact(const physx::PxContactPair& contact_pair) override {};
    void onTrigger(bool trigger_enter, physx::PxShape* actor_shape, physx::PxShape* contact_shape) override {};
  protected:
    CollisionHandler() = default;
  };
}