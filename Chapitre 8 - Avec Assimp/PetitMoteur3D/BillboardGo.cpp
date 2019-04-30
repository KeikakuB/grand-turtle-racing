#include "stdafx.h"
#include "BillboardGo.h"

#include "PoseComponent.h"
#include "BillboardRenderComponent.h"

namespace PM3D
{
  BillboardGo::BillboardGo()
    : dx_{ 30 }, dy_{ 30 }, dz_{ 30 }
  {
  }

  BillboardGo::BillboardGo(float dx, float dy, float dz)
    : dx_{ dx }, dy_{ dy }, dz_{ dz }
  {
  }

  void PM3D::BillboardGo::OnSpawn(const physx::PxTransform& pose)
  {
    AddComponent<PoseComponent>(pose);
    boost::filesystem::path texture_path{ ".\\data\\UneTexture.dds" };

    AddComponent<BillboardRenderComponent>(texture_path, dx_, dy_, dz_)->SetLodOptimizability(false);
  }

  BillboardGo::~BillboardGo()
  {
  }
}