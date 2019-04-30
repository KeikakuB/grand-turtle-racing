#pragma once
#include "Meta.h"
#include "GameObjectComponent.h"
#include "IUpdateComponentInterface.h"
#include "ITimeManager.h"
namespace PM3D
{
  class TurtleUpdateComponent :
    public GameObjectComponent,
    public IUpdateComponentInterface
  {
  public:
    TurtleUpdateComponent();
    ~TurtleUpdateComponent();
    virtual void Update() override;
  private:
    ITimeManager::time_point last_time_switched_camera_;
    int last_cam_id;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(TurtleUpdateComponent);
  }
}