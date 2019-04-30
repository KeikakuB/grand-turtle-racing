#include "stdafx.h"
#include "UpdateManager.h"
#include "GameSingleton.h"
#include "IUpdateComponentInterface.h"
#include "IWorldManager.h"

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(UpdateManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<UpdateManager>() noexcept { return{ GetInterfaceId<IUpdateManager>() }; }
  }
  UpdateManager::UpdateManager() = default;
  void UpdateManager::RunTask()
  {
    auto& game = GameSingleton::Get();
    for ( auto& s : Game<IWorldManager>()->GetGameObjects() )
    {
      auto updateInterface = s->As<IUpdateComponentInterface>();
      if ( updateInterface )
      {
        updateInterface->Update();
      }
    }
  }
}