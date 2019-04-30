#include "stdafx.h"
#include "Camera.h"
#include "PoseComponent.h"
#include "GameSingleton.h"

using namespace DirectX;

namespace PM3D
{
  Camera::Camera() : fov_{ XM_PI / 4 }, close_plane_{ 0.05f }, far_plane_{ 6000.0f }
  {
  }

  Camera::Camera(float fov, float close_plane, float far_plane)
    : fov_{ fov }, close_plane_{ close_plane }, far_plane_{ far_plane }
  {
  }

  void Camera::OnSpawn(const physx::PxTransform& pose)
  {
    LookAt(XMVectorSet(0.0f, -150.0f, 50.0f, 1.0f),
      XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
      XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
  }

  void Camera::LookAt(DirectX::FXMVECTOR eye, DirectX::FXMVECTOR focus, DirectX::FXMVECTOR up)
  {
    eye_position_ = DirectX::XMVECTOR{ eye };
    focus_position_ = DirectX::XMVECTOR{ focus };
    up_vector_ = DirectX::XMVECTOR{ up };
    mat_view_ = XMMatrixLookAtRH(eye_position_, focus, up);
    UpdateProjectionMatrix();
  }

  void Camera::UpdateProjectionMatrix()
  {
    auto pDispositif = GameSingleton::Get().GetDispositif();
    float ratioDAspect = pDispositif->GetLargeur() / pDispositif->GetHauteur();

    mat_proj_ = XMMatrixPerspectiveFovRH(
      fov_,
      ratioDAspect,
      close_plane_,
      far_plane_);

    // Calcul de VP à l'avance
    mat_view_proj_ = mat_view_ * mat_proj_;
  }
}