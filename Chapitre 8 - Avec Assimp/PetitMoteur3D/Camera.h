#pragma once
#include "GameObject.h"
#include "ICamera.h"
namespace PM3D
{
  class Camera : public GameObject, public ICamera
  {
  public:
    Camera();
    Camera(float fov, float close_plane, float far_plane);
    void LookAt(DirectX::FXMVECTOR eye, DirectX::FXMVECTOR focus, DirectX::FXMVECTOR up) override;
    DirectX::XMVECTOR GetEye() const override { return eye_position_; };
    DirectX::XMVECTOR GetFocus() const override { return focus_position_; };
    DirectX::XMVECTOR GetUp() const override { return up_vector_; };
    DirectX::XMMATRIX GetMatView() const { return mat_view_; }
    DirectX::XMMATRIX GetMatProj() const { return mat_proj_; }
    DirectX::XMMATRIX GetMatViewProj() const { return mat_view_proj_; }
    virtual void OnSpawn(const physx::PxTransform& pose) override;
  private:
    void UpdateProjectionMatrix();
    //field of view in radians
    float fov_;
    float close_plane_;
    float far_plane_;
    DirectX::XMMATRIX mat_view_;
    DirectX::XMMATRIX mat_proj_;
    DirectX::XMMATRIX mat_view_proj_;
    DirectX::XMVECTOR eye_position_;
    DirectX::XMVECTOR focus_position_;
    DirectX::XMVECTOR up_vector_;
  };
}