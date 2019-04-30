#pragma once
#include "Meta.h"
#include "ComponentObject.h"
#include "GameObjectComponent.h"
#include "IGameObject.h"

namespace PM3D
{
  class GameObject : public ComponentObject<GameObjectComponent>, public IGameObject
  {
  public:
    GameObject();
    virtual ~GameObject() = default;
    id_type GetId() const override final;
    void OnSpawn(const physx::PxTransform& pose) override;
    void OnUnspawn() override;
  private:
    const id_type id_;
    static id_type id_counter;
  };
  bool operator==(const GameObject &a, const GameObject &b);
}