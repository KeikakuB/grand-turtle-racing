#pragma once
#include "Meta.h"
#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
  class IRigidSimulationComponentInterface
  {
  public:
    virtual physx::PxRigidActor& pxActor() = 0;
    virtual ~IRigidSimulationComponentInterface() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IRigidSimulationComponentInterface);
  }
}
