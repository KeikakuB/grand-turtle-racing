#pragma once
#include "Meta.h"
#include "Light.h"
#include "GameObject.h"

#include <memory>

namespace PM3D
{
  class IWorldManager
  {
  public:
    virtual GameObject* SpawnGameObject(std::unique_ptr<GameObject> object, physx::PxTransform t) = 0;
    virtual void UnspawnGameObject(GameObject* object) = 0;
    virtual void SetLight(physx::PxTransform t) = 0;
    virtual std::vector<GameObject*> GetGameObjects() = 0;
    virtual DirectX::XMVECTOR GetLightPosition() const = 0;
    virtual GameObject* GetGameObjectById(GameObject::id_type id) = 0;
    virtual ~IWorldManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IWorldManager);
  }
}