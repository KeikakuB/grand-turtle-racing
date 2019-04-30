#include "stdafx.h"
#include "SimulationComponentBase.h"

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(DynamicSimulationComponent);
    template<>
    component_interface_id_list_t GetComponentInterfaces<DynamicSimulationComponent>() noexcept
    {
      return{
        GetInterfaceId<IPoseComponentInterface>(),
        GetInterfaceId<IColliderComponentInterface>(),
        GetInterfaceId<IRigidSimulationComponentInterface>(),
        GetInterfaceId<IDynamicSimulationComponentInterface>()
      };
    }
    REGISTER_COMPONENT_CPP_ID(StaticSimulationComponent);
    template<>
    component_interface_id_list_t GetComponentInterfaces<StaticSimulationComponent>() noexcept
    {
      return{
        GetInterfaceId<IPoseComponentInterface>(),
        GetInterfaceId<IColliderComponentInterface>(),
        GetInterfaceId<IRigidSimulationComponentInterface>(),
        GetInterfaceId<IStaticSimulationComponentInterface>()
      };
    }
  }
}