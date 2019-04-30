#include "stdafx.h"
#include "SkyboxGo.h"

#include "PoseComponent.h"
#include "SkyboxRenderComponent.h"

namespace PM3D
{
  SkyboxGo::SkyboxGo() = default;

  void SkyboxGo::OnSpawn(const physx::PxTransform& pose)
  {
    boost::filesystem::path texture_path{ ".\\data\\SkyBox\\" };

    AddComponent<SkyboxRenderComponent>(texture_path);
  }

  SkyboxGo::~SkyboxGo() = default;
}