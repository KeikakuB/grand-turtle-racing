#include "stdafx.h"
#include "Sandbank.h"
#include "SimulationComponentBase.h"
#include "BlockRenderComponent.h"
#include "BillboardRenderComponent.h"
#include "CollisionTypes.h"

using namespace physx;

namespace PM3D
{
  Sandbank::Sandbank() : Sandbank(1000, 1000, 10)
  {
  }

  Sandbank::Sandbank(float dx, float dy, float dz)
    : dx_{ dx }, dy_{ dy }, dz_{ dz }
  {
  }

  void Sandbank::OnSpawn(const physx::PxTransform& pose)
  {
    auto simulationComponent = AddComponent<StaticSimulationComponent>();
    simulationComponent->SetTransform(pose);

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

    boost::filesystem::path texture_path{ ".\\data\\sand.dds" };

    AddComponent<BillboardRenderComponent>(texture_path, block_size.x, block_size.y, block_size.z)->SetLodOptimizability(false);
  }
}