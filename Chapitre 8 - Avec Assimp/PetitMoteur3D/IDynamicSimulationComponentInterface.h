#pragma once
#include "Meta.h"
#include "IRigidSimulationComponentInterface.h"

#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
  class IDynamicSimulationComponentInterface :
    virtual public IRigidSimulationComponentInterface
  {
  public:
    virtual physx::PxRigidDynamic& pxActor() = 0;
    virtual ~IDynamicSimulationComponentInterface() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IDynamicSimulationComponentInterface);
  }
}
