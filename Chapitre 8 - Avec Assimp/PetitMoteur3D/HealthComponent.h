#pragma once
#include "Meta.h"
#include "IHealthComponentInterface.h"
#include "GameObjectComponent.h"

namespace PM3D
{
  class HealthComponent final :
    public GameObjectComponent, public IHealthComponentInterface
  {
  public:
    HealthComponent();
    HealthComponent(int health);
    void SetHealth(int health) override;
    int GetHealth() const override;
    void ChangeHealth(int delta) override;
  private:
    int health_;
    int max_health;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(HealthComponent);
  }
}
