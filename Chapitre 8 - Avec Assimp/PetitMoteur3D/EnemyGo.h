#pragma once
#include "GameObject.h"

namespace PM3D
{
  class EnemyGo final : public GameObject
  {
  public:
    EnemyGo();
    void OnSpawn(const physx::PxTransform& pose) override;
  };
}