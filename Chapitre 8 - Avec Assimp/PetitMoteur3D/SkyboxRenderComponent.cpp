#include "stdafx.h"
#include "SkyboxRenderComponent.h"
#include "sommetbloc.h"
#include "util.h"
#include "resource.h"
#include "GameSingleton.h"
#include "IPoseComponentInterface.h"
#include "ICameraManager.h"
#include "LoaderSingleton.h"
#include "Math.h"
#include "Ids.h"
#include "ITextureManager.h"

#include "IWorldManager.h"

using namespace DirectX;
using namespace UtilitairesDX;

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(SkyboxRenderComponent);
    template<>
    component_interface_id_list_t GetComponentInterfaces<SkyboxRenderComponent>() noexcept { return{ GetInterfaceId<IRenderComponentInterface>() }; }
  }
  struct ShadersParams
  {
    DirectX::XMMATRIX matWorldViewProj;	// la matrice totale
    DirectX::XMMATRIX matWorld;			// matrice de transformation dans le monde
  };
  SkyboxRenderComponent::SkyboxRenderComponent(const boost::filesystem::path& filename)
    : SkyboxRenderComponent(filename, 1, 1, 1)
  {
  }

  SkyboxRenderComponent::SkyboxRenderComponent(const boost::filesystem::path& filename, float dx, float dy, float dz)
  {
    pSampleState = 0;

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
    XMFLOAT3 n1(0.0f, 0.0f, 1.0f); // arri�re
    XMFLOAT3 n2(0.0f, -1.0f, 0.0f); // dessous
    XMFLOAT3 n3(0.0f, 1.0f, 0.0f); // dessus
    XMFLOAT3 n4(-1.0f, 0.0f, 0.0f); // face gauche
    XMFLOAT3 n5(1.0f, 0.0f, 0.0f); // face droite

    CSommetBloc sommets[ 24 ];

    // Le devant du bloc
    sommets[ 0 ] = CSommetBloc(point[ 0 ], n0, XMFLOAT2(0.0f, 0.0f));
    sommets[ 1 ] = CSommetBloc(point[ 1 ], n0, XMFLOAT2(1.0f, 0.0f));
    sommets[ 2 ] = CSommetBloc(point[ 2 ], n0, XMFLOAT2(1.0f, 1.0f));
    sommets[ 3 ] = CSommetBloc(point[ 3 ], n0, XMFLOAT2(0.0f, 1.0f));

    // L'arri�re du bloc
    sommets[ 4 ] = CSommetBloc(point[ 4 ], n1, XMFLOAT2(0.0f, 1.0f));
    sommets[ 5 ] = CSommetBloc(point[ 5 ], n1, XMFLOAT2(0.0f, 0.0f));
    sommets[ 6 ] = CSommetBloc(point[ 6 ], n1, XMFLOAT2(1.0f, 0.0f));
    sommets[ 7 ] = CSommetBloc(point[ 7 ], n1, XMFLOAT2(1.0f, 1.0f));

    // Le dessous du bloc
    sommets[ 8 ] = CSommetBloc(point[ 3 ], n2, XMFLOAT2(0.0f, 0.0f));
    sommets[ 9 ] = CSommetBloc(point[ 2 ], n2, XMFLOAT2(1.0f, 0.0f));
    sommets[ 10 ] = CSommetBloc(point[ 6 ], n2, XMFLOAT2(1.0f, 1.0f));
    sommets[ 11 ] = CSommetBloc(point[ 5 ], n2, XMFLOAT2(0.0f, 1.0f));

    // Le dessus du bloc
    sommets[ 12 ] = CSommetBloc(point[ 0 ], n3, XMFLOAT2(0.0f, 1.0f));
    sommets[ 13 ] = CSommetBloc(point[ 4 ], n3, XMFLOAT2(0.0f, 0.0f));
    sommets[ 14 ] = CSommetBloc(point[ 7 ], n3, XMFLOAT2(1.0f, 0.0f));
    sommets[ 15 ] = CSommetBloc(point[ 1 ], n3, XMFLOAT2(1.0f, 1.0f));

    // La face gauche
    sommets[ 16 ] = CSommetBloc(point[ 0 ], n4, XMFLOAT2(0.0f, 0.0f));
    sommets[ 17 ] = CSommetBloc(point[ 3 ], n4, XMFLOAT2(1.0f, 0.0f));
    sommets[ 18 ] = CSommetBloc(point[ 5 ], n4, XMFLOAT2(1.0f, 1.0f));
    sommets[ 19 ] = CSommetBloc(point[ 4 ], n4, XMFLOAT2(0.0f, 1.0f));

    // La face droite
    sommets[ 20 ] = CSommetBloc(point[ 1 ], n5, XMFLOAT2(0.0f, 0.0f));
    sommets[ 21 ] = CSommetBloc(point[ 7 ], n5, XMFLOAT2(1.0f, 0.0f));
    sommets[ 22 ] = CSommetBloc(point[ 6 ], n5, XMFLOAT2(1.0f, 1.0f));
    sommets[ 23 ] = CSommetBloc(point[ 2 ], n5, XMFLOAT2(0.0f, 1.0f));

    // Cr�ation du vertex buffer et copie des sommets
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

    // Cr�ation de l'index buffer et copie des indices
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned int) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = index_skybox;
    pIndexBuffer = NULL;

    DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
      DXE_CREATIONINDEXBUFFER);

    // Initialisation de l'effet
    InitEffet();

    auto textureManager = Game<ITextureManager>();
    if ( textureManager )
    {
      {
        boost::filesystem::path face = filename;
        face.append("front.dds");
        pTextureD3DFront = textureManager->GetNewTexture(face)->GetD3DTexture();
      }
      {
        boost::filesystem::path face = filename;
        face.append("back.dds");
        pTextureD3DBack = textureManager->GetNewTexture(face)->GetD3DTexture();
      }
      {
        boost::filesystem::path face = filename;
        face.append("left.dds");
        pTextureD3DLeft = textureManager->GetNewTexture(face)->GetD3DTexture();
      }
      {
        boost::filesystem::path face = filename;
        face.append("right.dds");
        pTextureD3DRight = textureManager->GetNewTexture(face)->GetD3DTexture();
      }
      {
        boost::filesystem::path face = filename;
        face.append("up.dds");
        pTextureD3DUp = textureManager->GetNewTexture(face)->GetD3DTexture();
      }
      {
        boost::filesystem::path face = filename;
        face.append("down.dds");
        pTextureD3DDown = textureManager->GetNewTexture(face)->GetD3DTexture();
      }
    }
  }

  SkyboxRenderComponent::~SkyboxRenderComponent()
  {
    DXRelacher(pSampleState);

    DXRelacher(pEffet);
    DXRelacher(pVertexLayout);
    DXRelacher(pIndexBuffer);
    DXRelacher(pVertexBuffer);
  }

  void SkyboxRenderComponent::Draw() const
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

    // input layout des sommets
    pImmediateContext->IASetInputLayout(pVertexLayout);

    auto cam = Game<ICameraManager>()->GetMainCamera();
    // Initialiser et sélectionner les «constantes» du VS
    ShadersParams sp;
    XMMATRIX viewProj = cam->GetMatViewProj();

    XMMATRIX matWorld = math::FromPxTransformToWorldMatrix(physx::PxTransform{ math::FromXmVectorToPxVec3(cam->GetEye()) });

    sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
    sp.matWorld = XMMatrixTranspose(matWorld);

    pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);

    ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");  // Nous n'avons qu'un seul CBuffer
    pCB->SetConstantBuffer(pConstantBuffer);

    // Activation de la texture
    {
      ID3DX11EffectShaderResourceVariable* variableTexture;
      variableTexture = pEffet->GetVariableByName("textureFront")->AsShaderResource();
      variableTexture->SetResource(pTextureD3DFront);
    }
    {
      ID3DX11EffectShaderResourceVariable* variableTexture;
      variableTexture = pEffet->GetVariableByName("textureBack")->AsShaderResource();
      variableTexture->SetResource(pTextureD3DBack);
    }
    {
      ID3DX11EffectShaderResourceVariable* variableTexture;
      variableTexture = pEffet->GetVariableByName("textureLeft")->AsShaderResource();
      variableTexture->SetResource(pTextureD3DLeft);
    }
    {
      ID3DX11EffectShaderResourceVariable* variableTexture;
      variableTexture = pEffet->GetVariableByName("textureRight")->AsShaderResource();
      variableTexture->SetResource(pTextureD3DRight);
    }
    {
      ID3DX11EffectShaderResourceVariable* variableTexture;
      variableTexture = pEffet->GetVariableByName("textureUp")->AsShaderResource();
      variableTexture->SetResource(pTextureD3DUp);
    }
    {
      ID3DX11EffectShaderResourceVariable* variableTexture;
      variableTexture = pEffet->GetVariableByName("textureDown")->AsShaderResource();
      variableTexture->SetResource(pTextureD3DDown);
    }
    // Le sampler state
    ID3DX11EffectSamplerVariable* variableSampler;
    variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, pSampleState);

    auto pDispositif = GameSingleton::Get().GetDispositif();

    pDispositif->DeactivateCulling();
    pDispositif->DeactivateZBuffer();

    // **** Rendu de l'objet
    pPasse->Apply(0, pImmediateContext);

    pImmediateContext->DrawIndexed(36, 0, 0);

    pDispositif->ActivateBackCulling();
    pDispositif->ActivateZBuffer();
  }

  void SkyboxRenderComponent::InitEffet()
  {
    LoaderSingleton::Get().CompileAndLoadEffectShader(
      ids::shaders_effect::kSkybox,
      &pSampleState,
      &pVertexLayout,
      CSommetBloc::layout.data(),
      CSommetBloc::layout.size(),
      sizeof(ShadersParams),
      &pConstantBuffer,
      &pEffet,
      &pTechnique,
      &pPasse);
  }

  bool SkyboxRenderComponent::CanOptimizeLod() const
  {
    return false;
  }
}