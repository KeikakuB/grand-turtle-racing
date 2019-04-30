#pragma once
#include "Meta.h"
#include "ICameraManager.h"
#include "GameManagerComponent.h"

#include <map>

namespace PM3D
{
  class CameraManager final : public GameManagerComponent, public ICameraManager
  {
  public:
    CameraManager();
    ICamera* GetCamera(int index) override;
    ICamera* GetMainCamera() override;
    int AddCamera(ICamera* camera) override;
    void SetMainCamera(int index) override;
    void RemoveCamera(int index) override;
    void RunTask() override;
  private:
    std::map<int, ICamera*> cameras_;
    int main_camera_index_;
    static int counter_;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(CameraManager);
  }
}
