#pragma once
#include "Meta.h"
#include "IWorldManager.h"
#include "GameManagerComponent.h"
#include "OctreeF.h"

#include <map>
#include <vector>
#include <memory>

namespace PM3D
{
  class no_such_game_object {};
  class game_object_already_added {};

  class WorldManager final :
    public GameManagerComponent, public IWorldManager
  {
  public:
    WorldManager();
    void RunTask() override;
    GameObject* SpawnGameObject(std::unique_ptr<GameObject> obj, physx::PxTransform t) override;

    template<typename T, class... Args> T* Spawn(physx::PxTransform T, Args&&... args);

    void UnspawnGameObject(GameObject* obj) override;
    void SetLight(physx::PxTransform t) override;
    std::vector<GameObject*> GetGameObjects() override;
    DirectX::XMVECTOR GetLightPosition() const override;
    virtual GameObject* GetGameObjectById(GameObject::id_type id) override;

    std::vector<OctreePoint<GameObject>*> getInsideLocalBox(physx::PxVec3 position);

  private:
    std::unique_ptr<Light> light_;
    std::map<GameObject::id_type, std::unique_ptr<GameObject>> objects_;

    std::unique_ptr<Octree<GameObject>> tree_;  // Pas instancié
    std::vector<OctreePoint<GameObject>*> treePoint_;
    physx::PxVec3 qMin = physx::PxVec3(100.0, 100.0, 100.0), qMax = physx::PxVec3(0.0, 0.0, 0.0);
    void init();
  };

  template<typename T, class... Args>
  T* WorldManager::Spawn(physx::PxTransform t, Args&&... args)
  {
    std::unique_ptr<T> ptr = std::make_unique<T>(std::forward<Args>(args)...);
    return dynamic_cast<T*>( SpawnGameObject(std::move(ptr), t) );
  }

  namespace meta
  {
    REGISTER_COMPONENT_H_ID(WorldManager);
  }
}