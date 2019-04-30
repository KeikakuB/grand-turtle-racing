#pragma once
#include "Meta.h"
#include "PxPhysicsAPI.forward.h"

#include "ComponentObject.h"
#include "GameObjectComponent.h"

namespace PM3D
{
  class ISimulationManager
  {
  public:
    using user_data_pointer = ComponentObject<GameObjectComponent>*;
    virtual physx::PxPhysics& physics() = 0;
    virtual physx::PxScene& scene() = 0;
    virtual ~ISimulationManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(ISimulationManager);
  }
}
