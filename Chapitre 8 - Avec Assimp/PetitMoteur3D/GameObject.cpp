#include "stdafx.h"
#include "GameObject.h"

namespace PM3D
{
  IGameObject::id_type GameObject::id_counter = 0;
  GameObject::GameObject() : id_{ ++id_counter } {}

  auto GameObject::GetId() const -> id_type
  {
    return id_;
  }

  void GameObject::OnSpawn(const physx::PxTransform& pose) {}

  void GameObject::OnUnspawn() {}

  bool operator==(const GameObject &a, const GameObject &b)
  {
    return a.GetId() == b.GetId();
  }
}