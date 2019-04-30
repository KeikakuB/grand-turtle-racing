#pragma once
#include "Camera.h"
namespace PM3D
{
  class FirstPersonCamera final :
    public Camera
  {
  public:
    FirstPersonCamera(GameObject::id_type target);
    ~FirstPersonCamera();
    virtual void OnSpawn(const physx::PxTransform& pose) override;
  private:
    GameObject::id_type target_;
  };
}
