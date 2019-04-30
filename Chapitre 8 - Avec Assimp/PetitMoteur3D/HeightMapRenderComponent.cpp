#include "stdafx.h"
#include "HeightMapRenderComponent.h"

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(HeightMapRenderComponent);
    template<>
    component_interface_id_list_t GetComponentInterfaces<HeightMapRenderComponent>() noexcept { return{ GetInterfaceId<IRenderComponentInterface>() }; }
  }
  HeightMapRenderComponent::HeightMapRenderComponent() = default;
  void HeightMapRenderComponent::Draw() const
  {
  }

  bool HeightMapRenderComponent::CanOptimizeLod() const
  {
    return false;
  }
}