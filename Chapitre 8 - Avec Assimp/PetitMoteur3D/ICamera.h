#pragma once
namespace PM3D
{
  class ICamera
  {
  public:
    virtual void LookAt(DirectX::FXMVECTOR eye, DirectX::FXMVECTOR focus, DirectX::FXMVECTOR up) = 0;
    virtual DirectX::XMVECTOR GetEye() const = 0;
    virtual DirectX::XMVECTOR GetFocus() const = 0;
    virtual DirectX::XMVECTOR GetUp() const = 0;
    virtual DirectX::XMMATRIX GetMatView() const = 0;
    virtual DirectX::XMMATRIX GetMatProj() const = 0;
    virtual DirectX::XMMATRIX GetMatViewProj() const = 0;
    virtual ~ICamera() = default;
  };
}