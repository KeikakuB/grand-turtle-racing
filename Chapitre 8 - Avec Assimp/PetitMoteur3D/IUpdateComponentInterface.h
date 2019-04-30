#pragma once
#include "Meta.h"
namespace PM3D
{
  class IUpdateComponentInterface
  {
  public:
    virtual void Update() = 0;
    virtual ~IUpdateComponentInterface() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IUpdateComponentInterface);
  }
}
