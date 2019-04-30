#include "stdafx.h"
#include "TurtleUpdateComponent.h"
#include "IInputManager.h"
#include "GameSingleton.h"
#include "IDynamicSimulationComponentInterface.h"
#include "IPoseComponentInterface.h"
#include "ICameraManager.h"
#include "IGameLogicManager.h"
#include "IPostEffectManager.h"
#include "Ids.h"
#include "IHealthComponentInterface.h"
#include "GameObject.h"

#include <iostream>
#include <dinput.h>
#include <PxPhysicsAPI.h>

#undef min

using namespace physx;
namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(TurtleUpdateComponent);
    template<>
    component_interface_id_list_t GetComponentInterfaces<TurtleUpdateComponent>() noexcept { return{ GetInterfaceId<IUpdateComponentInterface>() }; }
  }
  TurtleUpdateComponent::TurtleUpdateComponent()
    : last_time_switched_camera_{ ITimeManager::time_point::min() },
    last_cam_id{ 1 }
  {
  };
  TurtleUpdateComponent::~TurtleUpdateComponent() = default;

  void TurtleUpdateComponent::Update()
  {
    using namespace std::chrono_literals;
    static const float kForwardForce = 200000000.0f;
    static const float kBackwardForce = kForwardForce * 0.5f;
    static const float kTurnForce = 1000000000.0f;
    static const auto kCameraSwitchDelay = 300ms;
    static const short kNumberOfCameras = 2;
    auto turtle = GetGameObject();

    auto poseInterface = As<IPoseComponentInterface>();
    auto logic_manager = Game<IGameLogicManager>();
    if ( logic_manager->IsGameRestarted() )
    {
      auto pose = logic_manager->GetTurtlePose();
      if ( pose.isValid() )
      {
        poseInterface->SetTransform(pose);
        logic_manager->SetGameRestarted(false);
        auto simulation = As<IDynamicSimulationComponentInterface>();
        simulation->pxActor().setLinearVelocity(PxVec3{ 0,0,0 });
        simulation->pxActor().clearForce();
        simulation->pxActor().clearTorque();
        simulation->pxActor().setAngularVelocity(PxVec3{ 0,0,0 });
      }
    }
    else
    {
      auto input_manager = Game<IInputManager>();
      if ( input_manager )
      {
        PxTransform transform = poseInterface->GetTransform();
        auto& actor = As<IDynamicSimulationComponentInterface>()->pxActor();
        auto logic_manager = Game<IGameLogicManager>();
        if ( logic_manager )
        {
          logic_manager->UpdateTurtleVelocityValue(actor.getLinearVelocity().magnitude());
        }

        if ( !logic_manager->IsGamePause() )
        {
          PxVec3 front = poseInterface->GetTransform().q.rotate(PxVec3(0, -1, 0));
          PxVec3 left = poseInterface->GetTransform().q.rotate(PxVec3(1, 0, 0));
          PxVec3 up = poseInterface->GetTransform().q.rotate(PxVec3(0, 0, 1));

          if ( input_manager->IsKeyDown(KeyboardKey::UpArrow) )
          {
            turtle->As<IHealthComponentInterface>()->ChangeHealth(1);
            logic_manager->UpdateTurtleLifeValue(turtle->As<IHealthComponentInterface>()->GetHealth());
            PxVec3 frontVector = transform.q.rotate(PxVec3(0, -1, 0));
            transform.p += frontVector*8.0f;
            poseInterface->SetTransform(transform);

            /*
                        actor.clearForce();
                        actor.addForce(front * kForwardForce);
            */
          }
          else if ( input_manager->IsKeyDown(KeyboardKey::DownArrow) )
          {
            turtle->As<IHealthComponentInterface>()->ChangeHealth(1);
            logic_manager->UpdateTurtleLifeValue(turtle->As<IHealthComponentInterface>()->GetHealth());
            PxVec3 frontVector = transform.q.rotate(PxVec3(0, -1, 0));
            transform.p -= frontVector*6.0f;
            poseInterface->SetTransform(transform);

            /*
                        actor.clearForce();
                        actor.addForce(front * kBackwardForce * -1);*/
          }

          static float kRotateStep = 0.02f;
          if ( input_manager->IsKeyDown(KeyboardKey::LeftArrow) )
          {
            transform.q *= PxQuat(kRotateStep, PxVec3(0, 0, 1));
            poseInterface->SetTransform(transform);

            /*
                        actor.clearTorque();
                        actor.addTorque(up * kTurnForce);*/
          }
          else if ( input_manager->IsKeyDown(KeyboardKey::RightArrow) )
          {
            transform.q *= PxQuat(-kRotateStep, PxVec3(0, 0, 1));
            poseInterface->SetTransform(transform);

            /*

                        actor.clearTorque();
                        actor.addTorque(up * kTurnForce * -1);*/
          }
          if ( input_manager->IsKeyDown(KeyboardKey::L) )
          {
            turtle->As<IHealthComponentInterface>()->ChangeHealth(-500);
          }
          auto time_manager = Game<ITimeManager>();
          if ( time_manager )
          {
            auto now_time = time_manager->nowWorldTime();
            if ( input_manager->IsKeyDown(KeyboardKey::SpaceBar) && now_time - last_time_switched_camera_ > ITimeManager::duration{ kCameraSwitchDelay } )
            {
              last_time_switched_camera_ = now_time;
              auto camera_manager = Game<ICameraManager>();
              if ( camera_manager )
              {
                auto pe_manager = Game<IPostEffectManager>();
                if ( last_cam_id == 0 )
                {
                  pe_manager->EnqueueTechnique(ids::post_effect_techniques::kGrowingPeephole, 1500ms, 250ms);
                }
                else
                {
                  pe_manager->EnqueueTechnique(ids::post_effect_techniques::kHorizontalShuttersOpen, 1500ms, 250ms);
                }
                camera_manager->SetMainCamera(last_cam_id);
                last_cam_id = ( last_cam_id + 1 ) % kNumberOfCameras;
              }
            }
          }
        }
      }
    }
  }
}