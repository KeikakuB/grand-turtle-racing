#pragma once
#include "CollisionHandler.h"
namespace PM3D
{
  class TriggerZoneCollisionHandler :
    public CollisionHandler
  {
  public:
    TriggerZoneCollisionHandler();

    void onContact(const physx::PxContactPair& contact_pair) override;
    void onTrigger(bool trigger_enter, physx::PxShape* actor_shape, physx::PxShape* contact_shape) override {};
  };
}