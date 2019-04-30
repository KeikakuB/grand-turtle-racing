#include "stdafx.h"
#include "ObstacleGo.h"
#include "SimulationComponentBase.h"
#include "BlockRenderComponent.h"
#include "CelMeshRenderComponent.h"
#include "LambdaUpdateComponent.h"
#include "ISimulationManager.h"
#include "Ids.h"
#include "CollisionTypes.h"
#include "CollisionHandler.h"
#include <iostream>

using namespace physx;

namespace PM3D
{
  class CastleTunnelHandler : public CollisionHandler
  {
    virtual void onContact(const physx::PxContactPair &aContactPair) {}
    virtual void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape)
    {
      GameObject *actor = nullptr;

      if ( contactShape )
      {
        PxRigidActor *intruder = contactShape->getActor();
        actor = static_cast<GameObject*>( intruder->userData );
      }
      auto castle = actor->AsComponent<CelMeshRenderComponent>();
      if ( castle )
      {
        if ( triggerEnter )
        {
          castle->ChangeLighting(DirectX::XMVECTOR{ 0.5f, 0.5f, 0.5f, 0.5f });
        }
        else
        {
          castle->ChangeLighting(DirectX::XMVECTOR{ 2.0f, 2.0f, 2.0f, 2.0f });
        }
      }
    }
  };

  ObstacleGo::ObstacleGo() : ObstacleGo(10, 10, 10, CUBE)
  {
  }

  ObstacleGo::ObstacleGo(int obstacle) : ObstacleGo(10, 10, 10, obstacle)
  {
  }

  ObstacleGo::ObstacleGo(float dx, float dy, float dz, int obstacle)
    : dx_{ dx }, dy_{ dy }, dz_{ dz }, obstacle_{ obstacle }
  {
  }

  void ObstacleGo::OnSpawn(const physx::PxTransform& pose)
  {
    auto simulationComponent = AddComponent<StaticSimulationComponent>();
    simulationComponent->SetTransform(pose);

    auto &pxActor = simulationComponent->pxActor();

    std::string id;
    PxVec3 block_size;

    physx_material_ = physx::unique_ptr<PxMaterial>{
      Game<ISimulationManager>()->physics().createMaterial(1.0f, 1.0f, 1.0f)
    };

    PxFilterData filterData;
    filterData.word0 = CollisionTypes::kObstacle;
    filterData.word1 = CollisionTypes::kPlayer | CollisionTypes::kObstacle;

    PxShape *actorShape;

    if ( obstacle_ == CASTLE )
    {
      id = ids::models::kCastle;
      block_size = { 50, 100, 100 };
      PxVec3 trigger_size = { 170, 100, 100 };

      actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_, PxTransform{ 60.0f, 0, 0 });
      PxShape *actorShape2 = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_, PxTransform{ -60.0f, 0, 0 });
      PxShape *actorShape3 = pxActor.createShape(PxBoxGeometry(trigger_size / 2), *physx_material_);

      actorShape->setSimulationFilterData(filterData);
      actorShape2->setSimulationFilterData(filterData);
      actorShape3->setSimulationFilterData(filterData);
      actorShape3->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
      actorShape3->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

      AddComponent<CelMeshRenderComponent>(id)->SetLodOptimizability(false);
      simulationComponent->setHandler(std::make_unique<CastleTunnelHandler>());
    }
    else if ( obstacle_ == BUCKET )
    {
      id = ids::models::kBucket;
      block_size = { 40, 40, 40 };

      actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_);
      actorShape->setSimulationFilterData(filterData);
      AddComponent<CelMeshRenderComponent>(id);
    }
    else if ( obstacle_ == PALMTREE )
    {
      id = ids::models::kTree;
      block_size = { 10.0f, 10.0f, 30.0f };

      actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_);
      actorShape->setSimulationFilterData(filterData);
      AddComponent<CelMeshRenderComponent>(id);
    }
    else if ( obstacle_ == SHOVEL )
    {
      id = ids::models::kShovel;
      block_size = { 30, 40, 20 };

      actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_);
      actorShape->setSimulationFilterData(filterData);
      AddComponent<CelMeshRenderComponent>(id);
    }
    else if ( obstacle_ == RAKE )
    {
      id = ids::models::kRake;
      block_size = { 30, 40, 30 };

      actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_);
      actorShape->setSimulationFilterData(filterData);
      AddComponent<CelMeshRenderComponent>(id);
    }
    else if ( obstacle_ == SIFTER )
    {
      id = ids::models::kSifter;
      block_size = { 30.0f, 30.0f, 20.0f };

      actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_);
      actorShape->setSimulationFilterData(filterData);
      AddComponent<CelMeshRenderComponent>(id);
    }
    else if ( obstacle_ == WALL )
    {
      id = ids::models::kWall;
      block_size = { 400, 40, 200 };

      actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_);
      actorShape->setSimulationFilterData(filterData);
      AddComponent<CelMeshRenderComponent>(id)->SetLodOptimizability(false);
    }
    else
    {
      id = CUBE;
      block_size = { dx_, dy_, dz_ };

      actorShape = pxActor.createShape(PxBoxGeometry(block_size / 2), *physx_material_);
      actorShape->setSimulationFilterData(filterData);
      AddComponent<CelMeshRenderComponent>(id);
    }
  }
}