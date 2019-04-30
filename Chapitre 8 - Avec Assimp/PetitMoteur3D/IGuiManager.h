#pragma once
#include "Meta.h"
namespace PM3D
{
  class IGuiManager
  {
  public:

    virtual ~IGuiManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IGuiManager);
  }
}