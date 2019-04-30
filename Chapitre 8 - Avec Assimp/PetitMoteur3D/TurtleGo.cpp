#include "stdafx.h"
#include "TurtleGo.h"
#include "IPoseComponentInterface.h"
#include "SimulationComponentBase.h"
#include "HealthComponent.h"
#include "MeshRenderComponent.h"
#include "CelMeshRenderComponent.h"
#include "LambdaUpdateComponent.h"
#include "ISimulationManager.h"
#include "TurtleUpdateComponent.h"
#include "Ids.h"
#include "CollisionTypes.h"
#include "IGameLogicManager.h"

using namespace physx;

namespace PM3D
{
  TurtleGo::TurtleGo()
  {
  }

  void TurtleGo::OnSpawn(const physx::PxTransform& pose)
  {
    auto logic_manager = Game<IGameLogicManager>();
    if ( logic_manager )
    {
      logic_manager->SetTurtlePose(pose);
    }

    auto simulationComponent = AddComponent<DynamicSimulationComponent>();
    simulationComponent->SetTransform(pose);

    auto &pxActor = simulationComponent->pxActor();

    physx_material_ = physx::unique_ptr<PxMaterial>{
      Game<ISimulationManager>()->physics().createMaterial(1.0f, 1.0f, 0.1f)
    };

    PxVec3 block_size{ 30, 30, 10 };
    PxShape *actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_, PxTransform{ 0, 0, 0 });
    PxFilterData filterData;
    filterData.word0 = CollisionTypes::kPlayer;
    filterData.word1 = CollisionTypes::kTerrain | CollisionTypes::kObstacle;
    actorShape->setSimulationFilterData(filterData);
    PxRigidBodyExt::updateMassAndInertia(pxActor, 10, &PxVec3{ 0, 0, -15 });

    AddComponent<HealthComponent>(2000);
    AddComponent<TurtleUpdateComponent>();
    AddComponent<CelMeshRenderComponent>(ids::models::kTurtle)->SetLodOptimizability(false);
  }
}