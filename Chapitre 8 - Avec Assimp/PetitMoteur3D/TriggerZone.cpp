#include "stdafx.h"
#include "TriggerZone.h"
#include "SimulationComponentBase.h"
#include "BlockRenderComponent.h"
#include "BillboardRenderComponent.h"
#include "CollisionTypes.h"
#include "TriggerZoneCollisionHandler.h"

using namespace physx;

namespace PM3D
{
  TriggerZone::TriggerZone() : TriggerZone(100, 1000, 10)
  {
  }

  TriggerZone::TriggerZone(float dx, float dy, float dz)
    : dx_{ dx }, dy_{ dy }, dz_{ dz }
  {
  }

  void TriggerZone::OnSpawn(const physx::PxTransform& pose)
  {
    auto simulationComponent = AddComponent<StaticSimulationComponent>();
    simulationComponent->SetTransform(pose);

    simulationComponent->setHandler(std::make_unique<TriggerZoneCollisionHandler>());

    auto &pxActor = simulationComponent->pxActor();

    physx_material_ = physx::unique_ptr<PxMaterial>{
      Game<ISimulationManager>()->physics().createMaterial(0.5f, 0.5f, 0.1f)
    };

    PxVec3 block_size{ dx_, dy_, dz_ };
    PxShape *actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_);

    PxFilterData filterData;
    filterData.word0 = CollisionTypes::kTerrain;
    filterData.word1 = CollisionTypes::kPlayer | CollisionTypes::kObstacle;
    actorShape->setSimulationFilterData(filterData);

    boost::filesystem::path texture_path{ ".\\data\\water.dds" };

    AddComponent<BillboardRenderComponent>(texture_path, block_size.x, block_size.y, block_size.z)->SetLodOptimizability(false);
  }
}