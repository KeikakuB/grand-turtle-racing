#pragma once
#include "IComponent.h"
namespace PM3D
{
  class GameObjectComponent : public IComponent<GameObjectComponent>
  {
  public:
    virtual ~GameObjectComponent() = default;
    void OnAttached(ComponentObject<GameObjectComponent>& game_object) override { go_ = &game_object; };
    void OnDetached(ComponentObject<GameObjectComponent>& game_object) override {};
    template<typename T>
    T* As() { return go_->As<T>(); }
    template<typename T>
    const T* As() const { return go_->As<T>(); }
    ComponentObject<GameObjectComponent>* GetGameObject() { return go_; }
  protected:
    GameObjectComponent();

  private:
    ComponentObject<GameObjectComponent>* go_;
  };
}