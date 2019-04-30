#pragma once
#include "Meta.h"
#include "GameObjectComponent.h"
#include "ITargetComponentInterface.h"

namespace PM3D
{
  class TargetComponent final :
    public GameObjectComponent, public ITargetComponentInterface
  {
  public:
    TargetComponent(GameObject::id_type t);
    ~TargetComponent();
    virtual GameObject* GetTarget() override;
    virtual void SetTarget(GameObject::id_type t) override;
  private:
    GameObject::id_type target_id_;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(TargetComponent);
  }
}