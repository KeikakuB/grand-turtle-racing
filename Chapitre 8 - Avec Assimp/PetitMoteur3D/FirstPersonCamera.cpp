#include "stdafx.h"
#include "FirstPersonCamera.h"
#include "TargetComponent.h"
#include "LambdaUpdateComponent.h"
#include "IPoseComponentInterface.h"

#include "Math.h"
using namespace physx;

namespace PM3D
{
  FirstPersonCamera::FirstPersonCamera(GameObject::id_type target) : target_{ target }
  {
  }

  FirstPersonCamera::~FirstPersonCamera()
  {
  }

  void FirstPersonCamera::OnSpawn(const physx::PxTransform& pose)
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
          physx::PxVec3 eye_position{ t.p.x, t.p.y - 10, t.p.z + 5 };
          physx::PxVec3 front_vector = t.q.rotate(PxVec3(0, -1, 0));
          front_vector.normalize();
          physx::PxVec3 target_position = eye_position + ( 100 * front_vector );

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