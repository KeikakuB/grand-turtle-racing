#pragma once
#include <array>
class CSommetBloc
{
public:
  CSommetBloc() = default;
  CSommetBloc(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT3 _normal, DirectX::XMFLOAT2 _coordTex = DirectX::XMFLOAT2(0, 0));
  static std::array<D3D11_INPUT_ELEMENT_DESC, 3> layout;
private:
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT3 normal;
  DirectX::XMFLOAT2 coordTex;
};