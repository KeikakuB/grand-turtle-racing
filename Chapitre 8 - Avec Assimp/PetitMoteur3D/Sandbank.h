#pragma once
#include "GameObject.h"
namespace PM3D
{
  class Sandbank final : public GameObject
  {
  public:
    Sandbank();
    Sandbank(float dx, float dy, float dz);
    void OnSpawn(const physx::PxTransform& pose) override;
  private:
    float dx_, dy_, dz_;
    physx::unique_ptr<physx::PxMaterial> physx_material_;
  };
}