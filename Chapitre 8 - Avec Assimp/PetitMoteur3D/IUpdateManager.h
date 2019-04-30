#pragma once
#include "Meta.h"
namespace PM3D
{
  class IUpdateManager
  {
  public:
    virtual ~IUpdateManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IUpdateManager);
  }
}
