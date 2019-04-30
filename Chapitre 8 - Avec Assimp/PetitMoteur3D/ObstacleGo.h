#pragma once
#include "GameObject.h"

namespace PM3D
{
  class ObstacleGo final : public GameObject
  {
  public:
    ObstacleGo();
    ObstacleGo(int obstacle);
    ObstacleGo(float dx, float dy, float dz, int obstacle);
    void OnSpawn(const physx::PxTransform& pose) override;
    enum { CUBE, CASTLE, BUCKET, PALMTREE, SHOVEL, RAKE, SIFTER, WALL };
  private:
    float dx_, dy_, dz_;
    int obstacle_;
    physx::unique_ptr<physx::PxMaterial> physx_material_;
  };
}