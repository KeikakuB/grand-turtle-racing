#include "stdafx.h"
#include "InvisibleBox.h"
#include "SimulationComponentBase.h"
#include "BlockRenderComponent.h"

#include "CollisionTypes.h"

using namespace physx;

namespace PM3D
{
  InvisibleBox::InvisibleBox() : InvisibleBox(100, 10, 100)
  {
  }

  InvisibleBox::InvisibleBox(float dx, float dy, float dz)
    : dx_{ dx }, dy_{ dy }, dz_{ dz }
  {
  }

  void InvisibleBox::OnSpawn(const physx::PxTransform& pose)
  {
    auto simulationComponent = AddComponent<StaticSimulationComponent>();
    simulationComponent->SetTransform(pose);

    auto &pxActor = simulationComponent->pxActor();

    physx_material_ = physx::unique_ptr<PxMaterial>{
      Game<ISimulationManager>()->physics().createMaterial(0.5f, 0.5f, 0.1f)
    };

    PxVec3 block_size{ dx_,dy_,dz_ };
    PxShape *actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_);

    PxFilterData filterData;
    filterData.word0 = CollisionTypes::kTerrain;
    filterData.word1 = CollisionTypes::kPlayer | CollisionTypes::kObstacle;
    actorShape->setSimulationFilterData(filterData);
    //AddComponent<BlockRenderComponent>(block_size.x, block_size.y, block_size.z);
  }
}