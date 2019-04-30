#pragma once
#include "GameObject.h"
namespace PM3D
{
  class SkyboxGo :
    public GameObject
  {
  public:
    SkyboxGo();
    void OnSpawn(const physx::PxTransform& pose) override;
    ~SkyboxGo();
  private:
  };
}