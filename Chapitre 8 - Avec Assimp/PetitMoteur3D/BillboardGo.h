#pragma once
#include "GameObject.h"
namespace PM3D
{
  class BillboardGo :
    public GameObject
  {
  public:
    BillboardGo();
    BillboardGo(float dx, float dy, float dz);
    void OnSpawn(const physx::PxTransform& pose) override;
    ~BillboardGo();
  private:
    float dx_, dy_, dz_;
  };
}