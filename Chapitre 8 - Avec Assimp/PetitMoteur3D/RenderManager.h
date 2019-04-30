#pragma once
#include "Meta.h"
#include "IRenderManager.h"
#include "GameManagerComponent.h"

namespace PM3D
{
  class RenderManager final :
    public GameManagerComponent, public IRenderManager
  {
  public:
    RenderManager(std::function<void()> pre_task, std::function<void()> post_task);
    void RunTask() override;
  private:
    void SetPreTaskHook(std::function<void()> hook);
    void SetPostTaskHook(std::function<void()> hook);
    std::function<void()> pre_task_function_;
    std::function<void()> post_task_function_;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(RenderManager);
  }
}
