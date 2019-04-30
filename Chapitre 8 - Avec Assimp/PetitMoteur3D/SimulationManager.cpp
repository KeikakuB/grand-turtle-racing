#include "stdafx.h"
#include "SimulationManager.h"
#include "GameObject.h"
#include "IColliderComponentInterface.h"
#include "GameClock.h"
#include "GameSingleton.h"
#include "ITimeManager.h"

#include <chrono>

using namespace physx;
using namespace std::chrono;

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(SimulationManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<SimulationManager>() noexcept { return{ GetInterfaceId<ISimulationManager>() }; }
  }

  class SimulationEventCallback : public PxSimulationEventCallback
  {
    using ptr_t = ISimulationManager::user_data_pointer;
  public:
    virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override
    {
    }

    virtual void onWake(PxActor** actors, PxU32 count) override
    {
    }

    virtual void onSleep(PxActor** actors, PxU32 count) override
    {
    }

    virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
    {
      ptr_t actor0 = nullptr;
      if ( !( pairHeader.flags & PxContactPairHeaderFlag::eDELETED_ACTOR_0 ) )
        actor0 = static_cast<ptr_t>( pairHeader.actors[ 0 ]->userData );

      ptr_t actor1 = nullptr;
      if ( !( pairHeader.flags & PxContactPairHeaderFlag::eDELETED_ACTOR_1 ) )
        actor1 = static_cast<ptr_t>( pairHeader.actors[ 1 ]->userData );

      for ( int i = 0; i < (int) nbPairs; ++i )
      {
        if ( actor0 )
          actor0->As<IColliderComponentInterface>()->onContact(pairs[ i ]);

        if ( actor1 )
          actor1->As<IColliderComponentInterface>()->onContact(pairs[ i ]);
      }
    }

    virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override
    {
      for ( int i = 0; i < (int) count; ++i )
      {
        bool triggerEnter = false;
        if ( pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND )
          triggerEnter = true;
        else if ( pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST )
          triggerEnter = false;
        else
          continue;

        ptr_t trigger = nullptr;
        if ( !( pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER ) )
          trigger = static_cast<ptr_t>( pairs[ i ].triggerShape->getActor()->userData );

        ptr_t other = nullptr;
        if ( !( pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_OTHER ) )
          other = static_cast<ptr_t>( pairs[ i ].otherShape->getActor()->userData );

        if ( trigger )
        {
          auto collider = trigger->As<IColliderComponentInterface>();
          if ( collider )
            collider->onTrigger(triggerEnter, pairs[ i ].triggerShape, other ? pairs[ i ].otherShape : nullptr);
        }
        if ( other )
        {
          auto collider = other->As<IColliderComponentInterface>();
          collider->onTrigger(triggerEnter, pairs[ i ].otherShape, trigger ? pairs[ i ].triggerShape : nullptr);
        }
      }
    }
  };

  static PxDefaultErrorCallback gDefaultErrorCallback;
  static PxDefaultAllocator gDefaultAllocatorCallback;
  static SimulationEventCallback gDefaultSimulationCallback;

  SimulationManager::SimulationManager() = default;

  SimulationManager::~SimulationManager() = default;

  void SimulationManager::RunTask()
  {
    auto time_manager = Game<ITimeManager>();
    GameClock::duration remaining_to_simulate = time_manager->currentFrameTime() - time_manager->lastFrameTime();

    auto duration_step = 100ms;
    while ( remaining_to_simulate > 0ms )
    {
      if ( remaining_to_simulate > duration_step )
      {
        scene_->simulate(duration_cast<duration<PxReal>>( duration_step ).count());
        remaining_to_simulate -= duration_step;
      }
      else
      {
        scene_->simulate(duration_cast<duration<PxReal>>( remaining_to_simulate ).count());
        remaining_to_simulate = 0ms;
      }
      scene_->fetchResults(true);
    }
  }

  physx::PxPhysics& SimulationManager::physics()
  {
    return *physics_;
  }

  physx::PxScene& SimulationManager::scene()
  {
    return *scene_;
  }

  void fatalError(const char *str)
  {
    printf("%s", str);
    throw str;
  }
  static PxFilterFlags SimulationFilterShader(
    PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
  {
    // let triggers through
    if ( PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1) )
    {
      pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
      return PxFilterFlag::eDEFAULT;
    }
    // generate contacts for all that were not filtered above
    pairFlags = PxPairFlag::eCONTACT_DEFAULT;

    // trigger the contact callback for pairs (A,B) where
    // the filtermask of A contains the ID of B and vice versa.
    if ( ( filterData0.word0 & filterData1.word1 ) || ( filterData1.word0 & filterData0.word1 ) )
    {
      pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
      return PxFilterFlag::eDEFAULT;
    }

    return PxFilterFlag::eSUPPRESS;
  }
  void customizeSceneDesc(PxSceneDesc *aSceneDesc)
  {
    aSceneDesc->gravity = PxVec3(0.0f, 0, -60.0f);
    aSceneDesc->filterShader = &SimulationFilterShader;
  }

  void SimulationManager::initialize()
  {
    foundation_ = physx::unique_ptr<PxFoundation>(
      PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback));
    if ( !foundation_ )
      fatalError("PxCreateFoundation failed!");

    bool recordMemoryAllocations = true;
    profile_zone_manager_ = physx::unique_ptr<PxProfileZoneManager>(
      &PxProfileZoneManager::createProfileZoneManager(foundation_.get())
      );
    if ( !profile_zone_manager_ )
      fatalError("PxProfileZoneManager::createProfileZoneManager failed!");

#ifdef PX_WINDOWS
    PxCudaContextManagerDesc cudaContextManagerDesc;
    cuda_context_manager_ = physx::unique_ptr<PxCudaContextManager>(
      PxCreateCudaContextManager(*foundation_, cudaContextManagerDesc, profile_zone_manager_.get()));
    if ( cuda_context_manager_ )
    {
      if ( !cuda_context_manager_->contextIsValid() )
      {
        cuda_context_manager_.reset();
      }
    }
#endif

    physics_ = physx::unique_ptr<PxPhysics>(
      PxCreatePhysics(PX_PHYSICS_VERSION, *foundation_,
        PxTolerancesScale(), recordMemoryAllocations, profile_zone_manager_.get()));
    if ( !physics_ )
      fatalError("PxCreatePhysics failed!");

    if ( !PxInitExtensions(*physics_) )
      fatalError("PxInitExtensions failed!");

    if ( physics_->getPvdConnectionManager() != nullptr )
    {
      PxVisualDebuggerConnectionFlags connectionFlags(PxVisualDebuggerExt::getAllConnectionFlags());
      visual_debugger_connection_ = physx::unique_ptr<debugger::comm::PvdConnection>(
        PxVisualDebuggerExt::createConnection(physics_->getPvdConnectionManager(),
          "localhost", 5425, 100, connectionFlags));
      if ( visual_debugger_connection_ == nullptr )
        printf("    NOT CONNECTED!\n");
      else
        printf("    CONNECTED!\n");
    }

    //-------------------------------------------------------------
    // create the scene
    PxSceneDesc sceneDesc(physics_->getTolerancesScale());
    customizeSceneDesc(&sceneDesc);

    if ( !sceneDesc.cpuDispatcher )
    {
      cpu_dispatcher_ = physx::unique_ptr<PxDefaultCpuDispatcher>(PxDefaultCpuDispatcherCreate(1));
      if ( !cpu_dispatcher_ )
        fatalError("PxDefaultCpuDispatcherCreate failed!");
      sceneDesc.cpuDispatcher = cpu_dispatcher_.get();
    }
    if ( !sceneDesc.filterShader )
      sceneDesc.filterShader = PxDefaultSimulationFilterShader;

#ifdef PX_WINDOWS
    if ( !sceneDesc.gpuDispatcher && cuda_context_manager_ )
    {
      sceneDesc.gpuDispatcher = cuda_context_manager_->getGpuDispatcher();
    }
#endif

    scene_ = physx::unique_ptr<PxScene>(physics_->createScene(sceneDesc));
    if ( !scene_ )
      fatalError("createScene failed!");

    scene_->setSimulationEventCallback(&gDefaultSimulationCallback);
  }

  void SimulationManager::cleanup()
  {
    scene_.reset();
    cpu_dispatcher_.reset();
    visual_debugger_connection_.reset();
    if ( physics_ )
    {
      PxCloseExtensions();
      physics_.reset();
    }
    cuda_context_manager_.reset();
    profile_zone_manager_.reset();
    foundation_.reset();
  }

  void SimulationManager::OnAttached(ComponentObject<GameManagerComponent>& game_object)
  {
    initialize();
  }

  void SimulationManager::OnDetached(ComponentObject<GameManagerComponent>& game_object)
  {
    cleanup();
  }
}