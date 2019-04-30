#include "stdafx.h"
#include "Math.h"

using namespace DirectX;

namespace PM3D
{
  namespace math
  {
    DirectX::XMMATRIX FromPxQuatToMatrix(const physx::PxQuat& q)
    {
      physx::PxMat44 m{ q };
      return DirectX::XMMATRIX{
        m[ 0 ][ 0 ],  m[ 0 ][ 1 ] , m[ 0 ][ 2 ] , m[ 0 ][ 3 ]
        , m[ 1 ][ 0 ] , m[ 1 ][ 1 ] , m[ 1 ][ 2 ] , m[ 1 ][ 3 ]
        , m[ 2 ][ 0 ] , m[ 2 ][ 1 ] , m[ 2 ][ 2 ] , m[ 2 ][ 3 ]
        , m[ 3 ][ 0 ] , m[ 3 ][ 1 ] , m[ 3 ][ 2 ] , m[ 3 ][ 3 ] };
    }

    DirectX::XMMATRIX FromPxTransformToWorldMatrix(const physx::PxTransform& t)
    {
      DirectX::XMMATRIX matWorld = XMMatrixIdentity();
      DirectX::XMMATRIX dx_rot = math::FromPxQuatToMatrix(t.q);
      matWorld *= dx_rot;
      physx::PxVec3 p = t.p;
      matWorld *= XMMatrixTranslation(p.x, p.y, p.z);
      return matWorld;
    }

    DirectX::XMVECTOR FromPxVec3ToXmVector(const physx::PxVec3& p)
    {
      return XMVECTOR{ p.x, p.y, p.z, 1.0f };
    }

    physx::PxVec3 FromXmVectorToPxVec3(DirectX::FXMVECTOR v)
    {
      return physx::PxVec3{ v.vector4_f32[ 0 ],v.vector4_f32[ 1 ],v.vector4_f32[ 2 ] };
    }
  }
}