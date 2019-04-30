#pragma once
#include "Meta.h"
#include "IPoseComponentInterface.h"
#include "GameObjectComponent.h"

#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
  class PoseComponent final :
    public GameObjectComponent, public IPoseComponentInterface
  {
  public:
    PoseComponent(physx::PxTransform t);
    physx::PxTransform GetTransform() const override { return transform_; }
    void SetTransform(const physx::PxTransform& t) override { transform_ = t; };
  private:
    physx::PxTransform transform_;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(PoseComponent);
  }
}