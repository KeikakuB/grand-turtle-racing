#include "stdafx.h"
#include "Light.h"

#include "PoseComponent.h"

namespace PM3D
{
  Light::Light()
  {
  }

  void Light::OnSpawn(const physx::PxTransform& pose)
  {
    AddComponent<PoseComponent>(pose);
  }
}