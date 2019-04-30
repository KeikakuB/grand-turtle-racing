#include "stdafx.h"
#include "EnemyGo.h"
#include "SimulationComponentBase.h"
#include "MeshRenderComponent.h"

namespace PM3D
{
  EnemyGo::EnemyGo()
  {
  }

  void EnemyGo::OnSpawn(const physx::PxTransform& pose)
  {
    auto simulationComponent = AddComponent<DynamicSimulationComponent>();
    simulationComponent->SetTransform(pose);
    //AddComponent<MeshRenderComponent>();
  }
}