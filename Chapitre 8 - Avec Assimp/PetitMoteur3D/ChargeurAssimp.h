#pragma once
#include "chargeur.h"
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h>           // Output data structure

namespace PM3D
{
  class CChargeurAssimp :
    public IChargeur
  {
  protected:
    // Objet de classe Importer (Assimp)
    Assimp::Importer* pImporter;
    const aiScene* scene;
    ModelSettings model_settings;

  public:
    CChargeurAssimp();
    ~CChargeurAssimp() = default;

    virtual void Chargement(const ModelSettings& param);

    virtual unsigned int GetNombreSubmesh();
    virtual unsigned int GetNombreSommetsSubmesh(unsigned int n);
    virtual DirectX::XMFLOAT3 GetPosition(int noMesh, int NoSommet);
    virtual DirectX::XMFLOAT2 GetCoordTex(int noMesh, int NoSommet);
    virtual DirectX::XMFLOAT3 GetNormale(int noMesh, int NoSommet);
    virtual unsigned int GetIndice(int noMesh, int noPoly, int NoIndice);
    virtual unsigned int GetNombrePolygonesSubmesh(int noMesh);

    virtual unsigned int GetNombreMaterial();

    virtual void GetMaterial(int _i, CMaterial& mat);

    virtual unsigned int GetMaterialIndex(int i);

    // À VERIFIER
  };
}