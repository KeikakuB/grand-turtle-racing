#include "StdAfx.h"
#include "sommetbloc.h"
using namespace DirectX;

// Definir l'organisation de notre sommet
std::array<D3D11_INPUT_ELEMENT_DESC, 3> CSommetBloc::layout =
{
  D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  D3D11_INPUT_ELEMENT_DESC{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

CSommetBloc::CSommetBloc(XMFLOAT3 _position, XMFLOAT3 _normal, XMFLOAT2 _coordTex)
{
  position = _position;
  normal = _normal;
  coordTex = _coordTex;
}