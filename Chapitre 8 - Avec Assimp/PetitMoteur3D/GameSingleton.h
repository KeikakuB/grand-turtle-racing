#pragma once
#include "Singleton.h"
#include "ComponentObject.h"
#include "GameManagerComponent.h"
#include "dispositifD3D11.h"

#include <memory>
#include <map>

namespace PM3D
{
  class manager_with_that_priority_already_added {};
  class GameSingleton final : public Singleton<GameSingleton>, ComponentObject<GameManagerComponent>
  {
  public:
    friend class Singleton<GameSingleton>;
    template<typename T, typename U>
    friend class CMoteur;
    friend class CMoteurWindows;
    template<typename T>
    friend T* Game();
    CDispositifD3D11* GetDispositif() { return disp_; };
  private:
    void Tick();
    template<typename T, class... Args>
    T* AddManagerComponent(int priority, Args&&... args)
    {
      T* s = AddComponent<T>(std::forward<Args>(args)...);
      if ( managers_.find(priority) != managers_.end() )
      {
        throw manager_with_that_priority_already_added{};
      }
      managers_[ priority ] = s;
      return s;
    }
    void SetDispositif(CDispositifD3D11* disp) { disp_ = disp; }
    GameSingleton();
    std::map<int, GameManagerComponent*> managers_;
    CDispositifD3D11* disp_;
  };
  template<typename T>
  T* Game()
  {
    return GameSingleton::Get().As<T>();
  };
}