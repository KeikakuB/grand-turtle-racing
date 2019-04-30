#include "stdafx.h"
#include "RenderManager.h"
#include "GameSingleton.h"
#include "IRenderComponentInterface.h"
#include "IPoseComponentInterface.h"
#include "IWorldManager.h"
#include "ICameraManager.h"
#include "ISpriteManager.h"
#include "IPostEffectManager.h"
#include "Math.h"

#include <PxPhysicsAPI.h>

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(RenderManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<RenderManager>() noexcept { return{ GetInterfaceId<IRenderManager>() }; }
  }
  RenderManager::RenderManager(std::function<void()> pre_task, std::function<void()> post_task)
  {
    SetPreTaskHook(pre_task);
    SetPostTaskHook(post_task);
  }

  void RenderManager::RunTask()
  {
    auto pe_manager = Game<IPostEffectManager>();
    pre_task_function_();
    if ( pe_manager )
    {
      pe_manager->StartPostEffect();
      pre_task_function_();
    }

    auto camera_manager = Game<ICameraManager>();
    //compute camera plane
    auto cam = camera_manager->GetMainCamera();
    DirectX::XMVECTOR eye = cam->GetEye();
    physx::PxVec3 plane_normal = math::FromXmVectorToPxVec3(cam->GetFocus() - eye);
    plane_normal.normalize();
    physx::PxVec3 point_on_plane = math::FromXmVectorToPxVec3(eye);
    physx::PxPlane camera_plane{ point_on_plane, plane_normal };
    for ( auto s : Game<IWorldManager>()->GetGameObjects() )
    {
      auto renderInterface = s->As<IRenderComponentInterface>();
      if ( renderInterface )
      {
        IPoseComponentInterface* poseInterface{};
        if ( renderInterface->CanOptimizeLod()                                         //can optimize away using position?
          && ( poseInterface = s->As<IPoseComponentInterface>() )                      //has pose component?
          && camera_plane.n.dot(poseInterface->GetTransform().p) + camera_plane.d < 0  //is object's position behind camera plane?
          )
        {
          continue;
        }
        renderInterface->Draw();
      }
    }
    Game<ISpriteManager>()->Draw();
    post_task_function_();
    if ( pe_manager )
    {
      pe_manager->EndPostEffect();
      pe_manager->DrawPostEffect();
      post_task_function_();
    }
  }

  void RenderManager::SetPreTaskHook(std::function<void()> hook)
  {
    pre_task_function_ = hook;
  }

  void RenderManager::SetPostTaskHook(std::function<void()> hook)
  {
    post_task_function_ = hook;
  }
}