#include "stdafx.h"
#include "PoseComponent.h"

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(PoseComponent);
    template<>
    component_interface_id_list_t GetComponentInterfaces<PoseComponent>() noexcept { return{ GetInterfaceId<IPoseComponentInterface>() }; }
  }
  PoseComponent::PoseComponent(physx::PxTransform t)
  {
    SetTransform(t);
  }
}