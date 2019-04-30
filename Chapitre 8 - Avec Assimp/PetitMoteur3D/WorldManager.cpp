#include "stdafx.h"
#include "WorldManager.h"

#include "IPoseComponentInterface.h"
#include "Math.h"

#include <algorithm>

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(WorldManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<WorldManager>() noexcept { return{ GetInterfaceId<IWorldManager>() }; }
  }
  WorldManager::WorldManager() = default;

  void WorldManager::RunTask() {}

  GameObject* WorldManager::SpawnGameObject(std::unique_ptr<GameObject> obj, physx::PxTransform t)
  {
    if ( treePoint_.size() == 0 )
    {
      treePoint_.reserve(objects_.size());
    }

    if ( objects_.find(obj->GetId()) != objects_.end() )
    {
      throw game_object_already_added{};
    }
    obj->OnSpawn(t);
    qMin = qMin.minimum(t.p);
    if ( qMin != t.p )
    {
      qMax = qMax.maximum(t.p);
    }
    treePoint_.push_back(new OctreePoint<GameObject>(obj.get(), t.p));
    return ( objects_[ obj->GetId() ] = std::move(obj) ).get();
  }

  void WorldManager::UnspawnGameObject(GameObject* obj)
  {
    if ( objects_.find(obj->GetId()) == objects_.end() )
    {
      throw no_such_game_object{};
    }
    objects_.erase(obj->GetId());
    obj->OnUnspawn();
  }

  void WorldManager::SetLight(physx::PxTransform t)
  {
    light_ = std::unique_ptr<Light>(new Light);
    light_->OnSpawn(t);
  }

  std::vector<GameObject*> WorldManager::GetGameObjects()
  {
    std::vector<GameObject*> vec;
    vec.reserve(vec.size());
    for ( auto& p : objects_ )
    {
      vec.push_back(p.second.get());
    }
    return vec;
  }

  DirectX::XMVECTOR WorldManager::GetLightPosition() const
  {
    static const DirectX::XMVECTOR zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    auto poseInterface = light_->As<IPoseComponentInterface>();
    if ( poseInterface )
    {
      physx::PxVec3 pos = poseInterface->GetTransform().p;
      return math::FromPxVec3ToXmVector(pos);
    }
    return zero;
  }

  GameObject* WorldManager::GetGameObjectById(GameObject::id_type id)
  {
    GameObject* res = nullptr;
    if ( objects_.find(id) != objects_.end() )
    {
      res = objects_[ id ].get();
    }
    return res;
  }

  void WorldManager::init()
  {
    tree_ = std::make_unique<Octree<GameObject>>(qMin, qMax);
    for ( int i = 0; i < treePoint_.size(); i++ )
    {
      tree_->insert(treePoint_.at(i));
    }
  }

  std::vector<OctreePoint<GameObject>*> WorldManager::getInsideLocalBox(physx::PxVec3 position)
  {
    physx::PxVec3 positionMin = position + physx::PxVec3(-0.5, -0.5, -0.5);
    physx::PxVec3 positionMax = position + physx::PxVec3(0.5, 0.5, 0.5);
    std::vector<OctreePoint<GameObject>*> results;
    tree_->getPointsInsideBox(positionMin, positionMax, results);
    return results;
  }
}