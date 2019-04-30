#pragma once
#include "IComponent.h"
#include "IGameManager.h"

namespace PM3D
{
  class GameManagerComponent :
    public IComponent<GameManagerComponent>, public IGameManager
  {
  public:
    virtual ~GameManagerComponent() = default;
    void OnAttached(ComponentObject<GameManagerComponent>& game_object) override {};
    void OnDetached(ComponentObject<GameManagerComponent>& game_object) override {};
  protected:
    GameManagerComponent() = default;
  };
}