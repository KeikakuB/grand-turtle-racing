#include "stdafx.h"
#include "HealthComponent.h"
#include "GameSingleton.h"
#include "IGameLogicManager.h"

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(HealthComponent);
    template<>
    component_interface_id_list_t GetComponentInterfaces<HealthComponent>() noexcept { return{ GetInterfaceId<IHealthComponentInterface>() }; }
  }
  HealthComponent::HealthComponent(int health) : health_{ health } { max_health = health; }
  HealthComponent::HealthComponent() : health_{} {}
  void HealthComponent::SetHealth(int life) { health_ = life; }
  int HealthComponent::GetHealth() const { return health_; }
  void HealthComponent::ChangeHealth(int delta)
  {
    health_ = health_ - delta;
    //health_ = ( max_health < health_ ) ? max_health : health_;
    if ( GetHealth() <= 0 )
    {
      auto logic_manager = Game<IGameLogicManager>();
      logic_manager->SetGameOver(true);
      SetHealth(max_health);
    }
  }
}