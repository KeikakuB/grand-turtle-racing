#pragma once
#include "Meta.h"
#include "ICamera.h"

#include <memory>
namespace PM3D
{
  class no_main_camera_available {};
  class no_such_camera_to_remove {};
  class duplicate_camera_added {};
  class ICameraManager
  {
  public:
    virtual ICamera* GetCamera(int index) = 0;
    virtual ICamera* GetMainCamera() = 0;
    virtual int AddCamera(ICamera* camera) = 0;
    virtual void SetMainCamera(int index) = 0;
    virtual void RemoveCamera(int index) = 0;
    virtual ~ICameraManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(ICameraManager);
  }
}