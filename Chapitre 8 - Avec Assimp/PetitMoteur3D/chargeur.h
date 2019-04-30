#pragma once

#include "Material.h"
#include "SettingsStructs.h"

namespace PM3D
{
  class IChargeur
  {
  public:
    IChargeur() = default;
    virtual ~IChargeur() = default;

    virtual void Chargement(const ModelSettings& param) = 0;

    virtual unsigned int GetNombreSubmesh() = 0;
    virtual unsigned int GetNombreSommetsSubmesh(unsigned int n) = 0;
    virtual DirectX::XMFLOAT3 GetPosition(int noMesh, int NoSommet) = 0;
    virtual DirectX::XMFLOAT2 GetCoordTex(int noMesh, int NoSommet) = 0;
    virtual DirectX::XMFLOAT3 GetNormale(int noMesh, int NoSommet) = 0;
    virtual unsigned int GetIndice(int noMesh, int noPoly, int NoIndice) = 0;
    virtual unsigned int GetNombrePolygonesSubmesh(int noMesh) = 0;

    virtual unsigned int GetNombreMaterial() = 0;
    virtual void GetMaterial(int _i, CMaterial& mat) = 0;

    virtual unsigned int GetMaterialIndex(int i) = 0;
  };
}
