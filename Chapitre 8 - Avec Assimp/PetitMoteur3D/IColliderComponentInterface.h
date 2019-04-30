#pragma once
#include "Meta.h"
#include "ICollisionHandler.h"
#include "PxPhysicsAPI.forward.h"

#include <memory>

namespace PM3D
{
  class IColliderComponentInterface
  {
  public:
    virtual void onContact(const physx::PxContactPair&) = 0;
    virtual void onTrigger(bool triggerEnter, physx::PxShape* actorShape, physx::PxShape* contactShape) = 0;
    virtual void setHandler(std::unique_ptr<ICollisionHandler> handler) = 0;
    virtual ~IColliderComponentInterface() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IColliderComponentInterface);
  }
}
