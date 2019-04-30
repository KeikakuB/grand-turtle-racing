#pragma once
#include "Meta.h"
namespace PM3D
{
  class IRenderManager
  {
  public:
    virtual ~IRenderManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IRenderManager);
  }
}