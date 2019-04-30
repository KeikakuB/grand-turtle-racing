#pragma once
#include "Camera.h"
namespace PM3D
{
  class ThirdPersonCamera final :
    public Camera
  {
  public:
    ThirdPersonCamera(GameObject::id_type target);
    ~ThirdPersonCamera();
    virtual void OnSpawn(const physx::PxTransform& pose) override;
  private:
    GameObject::id_type target_;
  };
}
