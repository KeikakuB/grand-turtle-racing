#pragma once
#include "Meta.h"
namespace PM3D
{
  class IRenderComponentInterface
  {
  public:
    virtual void Draw() const = 0;
    virtual bool CanOptimizeLod() const = 0;
    virtual ~IRenderComponentInterface() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IRenderComponentInterface);
  }
}