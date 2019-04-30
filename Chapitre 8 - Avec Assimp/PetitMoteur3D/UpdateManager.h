#pragma once
#include "Meta.h"
#include "IUpdateManager.h"
#include "GameManagerComponent.h"
namespace PM3D
{
  class UpdateManager final :
    public GameManagerComponent, public IUpdateManager
  {
  public:
    UpdateManager();
    void RunTask() override;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(UpdateManager);
  }
}