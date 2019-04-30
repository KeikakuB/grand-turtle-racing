#include "stdafx.h"
#include "CameraManager.h"

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(CameraManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<CameraManager>() noexcept { return{ GetInterfaceId<ICameraManager>() }; }
  }
  int CameraManager::counter_{};
  CameraManager::CameraManager() : main_camera_index_{ -1 }
  {
  };

  ICamera* CameraManager::GetCamera(int index)
  {
    return cameras_.at(index);
  }

  auto CameraManager::GetMainCamera() -> ICamera*
  {
    if ( cameras_.find(main_camera_index_) == cameras_.end() )
    {
      throw no_main_camera_available{};
    }
    return cameras_[ main_camera_index_ ];
  }

  int CameraManager::AddCamera(ICamera* cam)
  {
    int id = counter_;
    cameras_[ counter_ ] = cam;
    ++counter_;
    return id;
  }

  void CameraManager::SetMainCamera(int index)
  {
    main_camera_index_ = index;
  }

  void CameraManager::RemoveCamera(int index)
  {
    cameras_.erase(index);
  }

  void CameraManager::RunTask() {}
}