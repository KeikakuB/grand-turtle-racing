#pragma once
#include "Meta.h"
#include "ISimulationManager.h"
#include "GameManagerComponent.h"

#include "GameObjectComponent.h"
#include "ComponentObject.h"

#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
  class SimulationManager final :
    public GameManagerComponent, public ISimulationManager
  {
  public:
    SimulationManager();
    ~SimulationManager();
    virtual void OnAttached(ComponentObject<GameManagerComponent>& game_object) override;
    virtual void OnDetached(ComponentObject<GameManagerComponent>& game_object) override;
    void RunTask() override;
    virtual physx::PxPhysics& physics() override;
    virtual physx::PxScene& scene() override;
  private:
    void initialize();
    void cleanup();
    physx::unique_ptr<physx::PxFoundation> foundation_;
    physx::unique_ptr<physx::PxProfileZoneManager> profile_zone_manager_;
    physx::unique_ptr<physx::PxPhysics> physics_;
    physx::unique_ptr<physx::PxScene> scene_;
    physx::unique_ptr<physx::PxDefaultCpuDispatcher> cpu_dispatcher_;
    physx::unique_ptr<physx::PxCudaContextManager> cuda_context_manager_;
    physx::unique_ptr<physx::debugger::comm::PvdConnection> visual_debugger_connection_;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(SimulationManager);
  }
}