#include "stdafx.h"
#include "TargetComponent.h"

#include "IWorldManager.h"
#include "GameSingleton.h"

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(TargetComponent);
    template<>
    component_interface_id_list_t GetComponentInterfaces<TargetComponent>() noexcept { return{ GetInterfaceId<ITargetComponentInterface>() }; }
  }
  TargetComponent::TargetComponent(GameObject::id_type t) : target_id_{ t }
  {
  }

  TargetComponent::~TargetComponent() = default;

  GameObject* TargetComponent::GetTarget()
  {
    return Game<IWorldManager>()->GetGameObjectById(target_id_);
  }

  void TargetComponent::SetTarget(GameObject::id_type t)
  {
    target_id_ = t;
  }
}