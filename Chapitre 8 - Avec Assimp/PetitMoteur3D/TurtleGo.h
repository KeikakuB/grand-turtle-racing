#pragma once
#include "GameObject.h"

#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
  class TurtleGo final : public GameObject
  {
  public:
    TurtleGo();
    void OnSpawn(const physx::PxTransform& pose) override;

    physx::PxTransform pose_spawn_start_;
  private:
    physx::unique_ptr<physx::PxMaterial> physx_material_;
  };
}