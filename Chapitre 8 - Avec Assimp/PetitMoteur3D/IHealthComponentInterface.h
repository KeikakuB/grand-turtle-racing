#pragma once
#include "Meta.h"
namespace PM3D
{
  class IHealthComponentInterface
  {
  public:
    virtual void SetHealth(int health) = 0;
    virtual int GetHealth() const = 0;
    virtual void ChangeHealth(int delta) = 0;
    virtual ~IHealthComponentInterface() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IHealthComponentInterface);
  }
}