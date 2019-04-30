#include "stdafx.h"
#include "FoodGo.h"
#include "PoseComponent.h"
#include "SimulationComponentBase.h"
#include "MeshRenderComponent.h"
#include "CelMeshRenderComponent.h"
#include "LambdaUpdateComponent.h"
#include "IHealthComponentInterface.h"
#include "IWorldManager.h"
#include "ISimulationManager.h"
#include "Ids.h"
#include "CollisionTypes.h"
#include "CollisionHandler.h"
#include "IPostEffectManager.h"
#include <iostream>

using namespace physx;

namespace PM3D
{
  class FoodCollisionHandler : public CollisionHandler
  {
    virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
    {
    }

    virtual void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape)
    {
      using namespace std::chrono_literals;
      if ( triggerEnter )
      {
        GameObject *actor = nullptr;
        GameObject *turtle = nullptr;

        if ( actorShape )
        {
          PxRigidActor *intruder = actorShape->getActor();
          actor = static_cast<GameObject*>( intruder->userData );
        }
        if ( contactShape )
        {
          PxRigidActor *intruder = contactShape->getActor();
          turtle = static_cast<GameObject*>( intruder->userData );
        }
        if ( turtle )
        {
          turtle->As<IHealthComponentInterface>()->ChangeHealth(-200);
          auto post_effect = Game<IPostEffectManager>();
          post_effect->EnqueueTechnique(ids::post_effect_techniques::kTiles, 1000ms, 0ms);
        }
        if ( actor )
        {
          auto logic_manager = Game<IWorldManager>();
          actor->RemoveComponent<StaticSimulationComponent>();
          actor->RemoveComponent<CelMeshRenderComponent>();
        }
      }
    }
  };

  FoodGo::FoodGo() : FoodGo(30, 30, 30)
  {
  }

  FoodGo::FoodGo(float dx, float dy, float dz)
    : dx_{ dx }, dy_{ dy }, dz_{ dz }
  {
  }

  void FoodGo::OnSpawn(const physx::PxTransform& pose)
  {
    auto simulationComponent = AddComponent<StaticSimulationComponent>();
    simulationComponent->SetTransform(pose);

    auto &pxActor = simulationComponent->pxActor();

    physx_material_ = physx::unique_ptr<PxMaterial>{
      Game<ISimulationManager>()->physics().createMaterial(1.0f, 1.0f, 1.0f)
    };

    PxFilterData filterData;
    filterData.word0 = CollisionTypes::kObstacle;
    filterData.word1 = CollisionTypes::kPlayer | CollisionTypes::kTerrain | CollisionTypes::kObstacle;

    PxVec3 block_size = { dx_, dy_, dz_ };

    PxShape *actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_);

    actorShape->setSimulationFilterData(filterData);
    actorShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
    actorShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

    AddComponent<CelMeshRenderComponent>(ids::models::kJellyfish);
    simulationComponent->setHandler(std::make_unique<FoodCollisionHandler>());
  }
}