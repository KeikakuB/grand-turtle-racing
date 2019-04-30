#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "TargetComponent.h"
#include "LambdaUpdateComponent.h"
#include "IPoseComponentInterface.h"

#include "Math.h"
using namespace physx;

namespace PM3D
{
  ThirdPersonCamera::ThirdPersonCamera(GameObject::id_type target) : target_{ target }
  {
  }

  ThirdPersonCamera::~ThirdPersonCamera()
  {
  }

  void ThirdPersonCamera::OnSpawn(const physx::PxTransform& pose)
  {
    AddComponent<TargetComponent>(target_);
    auto third_cam_lambda = AddComponent<LambdaUpdateComponent>(
      [&]
    ()
    {
      auto targetInterface = this->As<ITargetComponentInterface>();
      if ( targetInterface )
      {
        auto poseInterface = targetInterface->GetTarget()->As<IPoseComponentInterface>();
        if ( poseInterface )
        {
          PxTransform t = poseInterface->GetTransform();
          physx::PxVec3 front_vector = t.q.rotate(PxVec3(0, -1, 0));
          front_vector.normalize();
          physx::PxVec3 back_vector = t.q.rotate(PxVec3(0, 1, 0));
          back_vector.normalize();
          physx::PxVec3 target_position = t.p + ( 50 * front_vector );
          physx::PxVec3 eye_position = t.p + ( 100 * back_vector );
          eye_position.z += 40;
          LookAt(
            math::FromPxVec3ToXmVector(eye_position),
            math::FromPxVec3ToXmVector(target_position),
            DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f));
        }
      }
    }
    );
  }
}