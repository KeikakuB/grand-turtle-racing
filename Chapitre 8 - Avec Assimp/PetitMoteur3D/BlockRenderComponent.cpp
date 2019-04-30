#include "stdafx.h"
#include "BlockRenderComponent.h"
#include "sommetbloc.h"
#include "util.h"
#include "resource.h"
#include "GameSingleton.h"
#include "IPoseComponentInterface.h"
#include "ICameraManager.h"
#include "LoaderSingleton.h"
#include "Math.h"
#include "Ids.h"

#include "IWorldManager.h"

using namespace DirectX;
using namespace UtilitairesDX;

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(BlockRenderComponent);
    template<>
    component_interface_id_list_t GetComponentInterfaces<BlockRenderComponent>() noexcept { return{ GetInterfaceId<IRenderComponentInterface>() }; }
  }
  struct ShadersParams
  {
    XMMATRIX matWorldViewProj;	// la matrice totale
    XMMATRIX matWorld;			// matrice de transformation dans le monde
    XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
    XMVECTOR vCamera; 			// la position de la caméra
    XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
    XMVECTOR vAMat; 			// la valeur ambiante du matériau
    XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage
    XMVECTOR vDMat; 			// la valeur diffuse du matériau
  };
  BlockRenderComponent::BlockRenderComponent() : BlockRenderComponent(1, 1, 1) {};
  BlockRenderComponent::BlockRenderComponent(float dx, float dy, float dz)
    : lod_optimizability_{ true }
  {
    // Les points
    XMFLOAT3 point[ 8 ];
    point[ 0 ] = XMFLOAT3(-dx / 2, dy / 2, -dz / 2);
    point[ 1 ] = XMFLOAT3(dx / 2, dy / 2, -dz / 2);
    point[ 2 ] = XMFLOAT3(dx / 2, -dy / 2, -dz / 2);
    point[ 3 ] = XMFLOAT3(-dx / 2, -dy / 2, -dz / 2);
    point[ 4 ] = XMFLOAT3(-dx / 2, dy / 2, dz / 2);
    point[ 5 ] = XMFLOAT3(-dx / 2, -dy / 2, dz / 2);
    point[ 6 ] = XMFLOAT3(dx / 2, -dy / 2, dz / 2);
    point[ 7 ] = XMFLOAT3(dx / 2, dy / 2, dz / 2);

    // Calculer les normales
    XMFLOAT3 n0(0.0f, 0.0f, -1.0f); // devant
    XMFLOAT3 n1(0.0f, 0.0f, 1.0f); // arrière
    XMFLOAT3 n2(0.0f, -1.0f, 0.0f); // dessous
    XMFLOAT3 n3(0.0f, 1.0f, 0.0f); // dessus
    XMFLOAT3 n4(-1.0f, 0.0f, 0.0f); // face gauche
    XMFLOAT3 n5(1.0f, 0.0f, 0.0f); // face droite

    CSommetBloc sommets[ 24 ];

    // Le devant du bloc
    sommets[ 0 ] = CSommetBloc(point[ 0 ], n0);
    sommets[ 1 ] = CSommetBloc(point[ 1 ], n0);
    sommets[ 2 ] = CSommetBloc(point[ 2 ], n0);
    sommets[ 3 ] = CSommetBloc(point[ 3 ], n0);

    // L'arrière du bloc
    sommets[ 4 ] = CSommetBloc(point[ 4 ], n1);
    sommets[ 5 ] = CSommetBloc(point[ 5 ], n1);
    sommets[ 6 ] = CSommetBloc(point[ 6 ], n1);
    sommets[ 7 ] = CSommetBloc(point[ 7 ], n1);

    // Le dessous du bloc
    sommets[ 8 ] = CSommetBloc(point[ 3 ], n2);
    sommets[ 9 ] = CSommetBloc(point[ 2 ], n2);
    sommets[ 10 ] = CSommetBloc(point[ 6 ], n2);
    sommets[ 11 ] = CSommetBloc(point[ 5 ], n2);

    // Le dessus du bloc
    sommets[ 12 ] = CSommetBloc(point[ 0 ], n3);
    sommets[ 13 ] = CSommetBloc(point[ 4 ], n3);
    sommets[ 14 ] = CSommetBloc(point[ 7 ], n3);
    sommets[ 15 ] = CSommetBloc(point[ 1 ], n3);
    // La face gauche
    sommets[ 16 ] = CSommetBloc(point[ 0 ], n4);
    sommets[ 17 ] = CSommetBloc(point[ 3 ], n4);
    sommets[ 18 ] = CSommetBloc(point[ 5 ], n4);
    sommets[ 19 ] = CSommetBloc(point[ 4 ], n4);

    // La face droite
    sommets[ 20 ] = CSommetBloc(point[ 1 ], n5);
    sommets[ 21 ] = CSommetBloc(point[ 7 ], n5);
    sommets[ 22 ] = CSommetBloc(point[ 6 ], n5);
    sommets[ 23 ] = CSommetBloc(point[ 2 ], n5);

    // Création du vertex buffer et copie des sommets
    ID3D11Device* pD3DDevice = GameSingleton::Get().GetDispositif()->GetD3DDevice();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CSommetBloc) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = sommets;
    pVertexBuffer = NULL;

    DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

    // Création de l'index buffer et copie des indices
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned int) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = index_bloc;
    pIndexBuffer = NULL;

    DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
      DXE_CREATIONINDEXBUFFER);

    // Initialisation des shaders
    InitShaders();
  }

  BlockRenderComponent::~BlockRenderComponent()
  {
    DXRelacher(pPixelShader);
    DXRelacher(pConstantBuffer);
    DXRelacher(pVertexLayout);
    DXRelacher(pVertexShader);
    DXRelacher(pIndexBuffer);
    DXRelacher(pVertexBuffer);
  }

  void BlockRenderComponent::Draw() const
  {
    // Obtenir le contexte
    ID3D11DeviceContext* pImmediateContext = GameSingleton::Get().GetDispositif()->GetImmediateContext();

    // Choisir la topologie des primitives
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Source des sommets
    UINT stride = sizeof(CSommetBloc);
    UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

    // Source des index
    pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Activer le VS
    pImmediateContext->VSSetShader(pVertexShader, NULL, 0);

    // input layout des sommets
    pImmediateContext->IASetInputLayout(pVertexLayout);

    auto cam = Game<ICameraManager>()->GetMainCamera();
    // Initialiser et sélectionner les «constantes» du VS
    ShadersParams sp;
    XMMATRIX viewProj = cam->GetMatViewProj();

    XMMATRIX matWorld = XMMatrixIdentity();
    auto poseInterface = As<IPoseComponentInterface>();
    if ( poseInterface )
    {
      matWorld = math::FromPxTransformToWorldMatrix(poseInterface->GetTransform());
    }

    sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
    sp.matWorld = XMMatrixTranspose(matWorld);
    sp.vLumiere = Game<IWorldManager>()->GetLightPosition();
    sp.vCamera = cam->GetEye();
    sp.vAEcl = DirectX::XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
    sp.vAMat = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
    sp.vDEcl = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
    sp.vDMat = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);

    pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);

    pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

    // Pas de Geometry Shader
    pImmediateContext->GSSetShader(NULL, NULL, 0);

    // Activer le PS
    pImmediateContext->PSSetShader(pPixelShader, NULL, 0);
    pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

    // **** Rendu de l'objet
    pImmediateContext->DrawIndexed(36, 0, 0);
  }

  void BlockRenderComponent::InitShaders()
  {
    LoaderSingleton::Get().CompileAndLoadVertexShader(
      ids::shaders_vertex::kMiniPhongVS,
      &pVertexShader,
      &pVertexLayout,
      CSommetBloc::layout.data(),
      CSommetBloc::layout.size(),
      sizeof(ShadersParams),
      &pConstantBuffer);

    LoaderSingleton::Get().CompileAndLoadPixelShader(
      ids::shaders_pixel::kMiniPhongPS,
      &pPixelShader);
  }

  bool BlockRenderComponent::CanOptimizeLod() const
  {
    return lod_optimizability_;
  }
}