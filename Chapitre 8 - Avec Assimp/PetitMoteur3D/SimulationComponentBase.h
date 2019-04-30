#pragma once
#include "Meta.h"
#include "GameObjectComponent.h"
#include "IPoseComponentInterface.h"
#include "IColliderComponentInterface.h"
#include "IRigidSimulationComponentInterface.h"
#include "IDynamicSimulationComponentInterface.h"
#include "IStaticSimulationComponentInterface.h"

#include "GameSingleton.h"
#include "ISimulationManager.h"

#include "PxPhysicsAPI.forward.h"

#include <memory>

namespace PM3D
{
  template<class TDerived, class PXACTORTYPE>
  class SimulationComponentBase :
    public GameObjectComponent,
    public IRigidSimulationComponentInterface,
    public IColliderComponentInterface,
    public IPoseComponentInterface
  {
  public:
    virtual void OnAttached(ComponentObject<GameObjectComponent>& game_object) override
    {
      GameObjectComponent::OnAttached(game_object);
      px_actor_ = TDerived::createPxActor();
      px_actor_->userData = static_cast<ISimulationManager::user_data_pointer>( &game_object );
      Game<ISimulationManager>()->scene().addActor(*px_actor_);
    }
    virtual void OnDetached(ComponentObject<GameObjectComponent>& game_object) override
    {
      GameObjectComponent::OnDetached(game_object);
      px_actor_.reset();
    }
    virtual void onContact(const physx::PxContactPair& aContactPair) override
    {
      if ( handler_ )
      {
        handler_->onContact(aContactPair);
      }
    }
    virtual void onTrigger(bool triggerEnter, physx::PxShape* actorShape, physx::PxShape* contactShape) override
    {
      if ( handler_ )
      {
        handler_->onTrigger(triggerEnter, actorShape, contactShape);
      }
    }
    virtual void setHandler(std::unique_ptr<ICollisionHandler> handler) override
    {
      handler_ = std::move(handler);
    }

  protected:
    physx::unique_ptr<PXACTORTYPE> px_actor_;
    std::unique_ptr<ICollisionHandler> handler_;
  };

  //=============================================================================
  // CLASS DynamicSimulationComponent
  //=============================================================================
  class DynamicSimulationComponent :
    public SimulationComponentBase<DynamicSimulationComponent, physx::PxRigidDynamic>,
    public IDynamicSimulationComponentInterface
  {
  public:
    static physx::unique_ptr<physx::PxRigidDynamic> createPxActor()
    {
      return physx::unique_ptr<physx::PxRigidDynamic>(
        Game<ISimulationManager>()->physics().createRigidDynamic(physx::PxTransform::createIdentity())
        );
    }
    virtual physx::PxTransform GetTransform() const override { return px_actor_->getGlobalPose(); };
    virtual void SetTransform(const physx::PxTransform& t) override { px_actor_->setGlobalPose(t); };

  public:
    virtual physx::PxRigidDynamic& pxActor() override { return *px_actor_; }
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(DynamicSimulationComponent);
  }

  //=============================================================================
  // CLASS StaticSimulationComponent
  //=============================================================================
  class StaticSimulationComponent :
    public SimulationComponentBase<StaticSimulationComponent, physx::PxRigidStatic>,
    public IStaticSimulationComponentInterface
  {
  public:
    static physx::unique_ptr<physx::PxRigidStatic> createPxActor()
    {
      return physx::unique_ptr<physx::PxRigidStatic>(
        Game<ISimulationManager>()->physics().createRigidStatic(physx::PxTransform::createIdentity())
        );
    }

  public: // IStaticSimulationInterface
    virtual physx::PxRigidStatic& pxActor() override { return *px_actor_; }

  public: // IPoseInterface
    virtual physx::PxTransform GetTransform() const override { return px_actor_->getGlobalPose(); };
    virtual void SetTransform(const physx::PxTransform& t) override { px_actor_->setGlobalPose(t); };
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(StaticSimulationComponent);
  }
}