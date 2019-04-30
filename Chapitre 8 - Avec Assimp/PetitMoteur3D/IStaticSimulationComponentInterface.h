#pragma once
#include "Meta.h"
#include "IRigidSimulationComponentInterface.h"

#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
  class IStaticSimulationComponentInterface :
    virtual public IRigidSimulationComponentInterface
  {
  public:
    virtual physx::PxRigidStatic& pxActor() = 0;
    virtual ~IStaticSimulationComponentInterface() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IStaticSimulationComponentInterface);
  }
}
