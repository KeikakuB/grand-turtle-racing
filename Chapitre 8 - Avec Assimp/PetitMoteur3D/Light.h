#pragma once
#include "GameObject.h"

namespace PM3D
{
  class Light final : public GameObject
  {
  public:
    Light();
    virtual void OnSpawn(const physx::PxTransform& pose) override;
  };
}