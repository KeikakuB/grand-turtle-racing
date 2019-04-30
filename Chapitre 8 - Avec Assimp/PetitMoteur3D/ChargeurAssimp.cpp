#include "stdafx.h"
#include "ChargeurAssimp.h"
#include <assimp/postprocess.h>     // Post processing flag
#include "boost\filesystem.hpp"
using namespace DirectX;

namespace PM3D
{
  CChargeurAssimp::CChargeurAssimp()
  {
    pImporter = nullptr;
  }

  void CChargeurAssimp::Chargement(const ModelSettings& param)
  {
    // Create an instance of the Importer class
    pImporter = new Assimp::Importer;

    model_settings = param;
    boost::filesystem::path the_path;
    the_path += param.relative_path;
    the_path += param.filename;

    int flags = 0;

    flags = aiProcess_CalcTangentSpace | aiProcess_FlipUVs |
      aiProcess_JoinIdenticalVertices |
      aiProcess_SortByPType;

    if ( param.left_hand ) flags |= aiProcess_MakeLeftHanded;
    if ( param.winding_order ) flags |= aiProcess_FlipWindingOrder;

    scene = pImporter->ReadFile(the_path.string(), flags);
  }

  unsigned int CChargeurAssimp::GetNombreSubmesh()
  {
    return scene->mNumMeshes;
  }

  unsigned int CChargeurAssimp::GetNombreSommetsSubmesh(unsigned int n)
  {
    return scene->mMeshes[ n ]->mNumVertices;
  }

  XMFLOAT3 CChargeurAssimp::GetPosition(int noMesh, int NoSommet)
  {
    aiVector3D v = scene->mMeshes[ noMesh ]->mVertices[ NoSommet ];
    XMFLOAT3 r(v.x, v.y, v.z);

    return r;
  }

  XMFLOAT2 CChargeurAssimp::GetCoordTex(int noMesh, int NoSommet)
  {
    scene->mMeshes[ noMesh ]->mTextureCoords[ NoSommet ]->x;
    XMFLOAT2 r(scene->mMeshes[ noMesh ]->mTextureCoords[ 0 ][ NoSommet ].x,
      scene->mMeshes[ noMesh ]->mTextureCoords[ 0 ][ NoSommet ].y);

    return r;
  }

  XMFLOAT3 CChargeurAssimp::GetNormale(int noMesh, int NoSommet)
  {
    aiVector3D v = scene->mMeshes[ noMesh ]->mNormals[ NoSommet ];
    XMFLOAT3 r(v.x, v.y, v.z);

    return r;
  }

  unsigned int CChargeurAssimp::GetIndice(int noMesh, int noPoly, int NoIndice)
  {
    return scene->mMeshes[ noMesh ]->mFaces[ noPoly ].mIndices[ NoIndice ];
  }

  unsigned int CChargeurAssimp::GetNombrePolygonesSubmesh(int noMesh)
  {
    return scene->mMeshes[ noMesh ]->mNumFaces;
  }

  unsigned int CChargeurAssimp::GetNombreMaterial()
  {
    return scene->mNumMaterials;
  }

  void CChargeurAssimp::GetMaterial(int _i, CMaterial& mat_data)
  {
    aiString name;
    name.Set(model_settings.texture.filename.string());
    scene->mMaterials[ _i ]->Get(AI_MATKEY_NAME, name);
    scene->mMaterials[ _i ]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), name);
    boost::filesystem::path texture_path = model_settings.relative_path;
    texture_path += model_settings.texture.filename;
    mat_data.NomFichierTexture = texture_path.string();
    aiColor3D color(0.f, 0.f, 0.f);
    scene->mMaterials[ _i ]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    mat_data.Diffuse = XMFLOAT4(color.r, color.g, color.b, 1.0f);
    scene->mMaterials[ _i ]->Get(AI_MATKEY_COLOR_AMBIENT, color);
    mat_data.Ambient = XMFLOAT4(color.r, color.g, color.b, 1.0f);
    scene->mMaterials[ _i ]->Get(AI_MATKEY_COLOR_SPECULAR, color);
    mat_data.Specular = XMFLOAT4(color.r, color.g, color.b, 1.0f);
    float puissance;
    scene->mMaterials[ _i ]->Get(AI_MATKEY_SHININESS, puissance);
    mat_data.Puissance = puissance;
  }

  unsigned int CChargeurAssimp::GetMaterialIndex(int i)
  {
    return scene->mMeshes[ i ]->mMaterialIndex;
  };
}