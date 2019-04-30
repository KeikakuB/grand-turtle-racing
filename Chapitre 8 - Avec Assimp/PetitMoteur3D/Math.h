#pragma once
#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
  namespace math
  {
    DirectX::XMMATRIX FromPxQuatToMatrix(const physx::PxQuat& q);
    DirectX::XMMATRIX FromPxTransformToWorldMatrix(const physx::PxTransform& t);
    DirectX::XMVECTOR FromPxVec3ToXmVector(const physx::PxVec3& p);
    physx::PxVec3 FromXmVectorToPxVec3(DirectX::FXMVECTOR v);
  }
}
