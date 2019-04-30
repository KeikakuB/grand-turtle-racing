#include "stdafx.h"
#include "CelMeshRenderComponent.h"
#include "ChargeurAssimp.h"
#include "ConfigSingleton.h"
#include "GameSingleton.h"
#include "util.h"
#include "resource.h"
#include "IPoseComponentInterface.h"
#include "ITextureManager.h"
#include "LoaderSingleton.h"
#include "ICameraManager.h"
#include "Math.h"
#include "Ids.h"

#include "IWorldManager.h"

using namespace DirectX;
using namespace UtilitairesDX;
using namespace std;

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(CelMeshRenderComponent);
    template<>
    component_interface_id_list_t GetComponentInterfaces<CelMeshRenderComponent>() noexcept { return{ GetInterfaceId<IRenderComponentInterface>() }; }
  }
  // Definir l'organisation de notre sommet
  std::array<D3D11_INPUT_ELEMENT_DESC, 3> CelMeshRenderComponent::MeshVertex::layout =
  {
    D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    D3D11_INPUT_ELEMENT_DESC{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  CelMeshRenderComponent::CelMeshRenderComponent(const std::string& id)
    : lod_optimizability_{ true }, model_settings_{ ConfigSingleton::Get().FetchById<ModelSettings>(id) }
  {
    CChargeurAssimp chargeur;
    chargeur.Chargement(model_settings_);  // Le chargeur lit le fichier

   // Placer l'objet sur la carte graphique
    TransfertObjet(chargeur);

    // Initialisation de l'effet
    InitEffet();
  }

  CelMeshRenderComponent::~CelMeshRenderComponent()
  {
    SubmeshMaterialIndex.clear();
    SubmeshIndex.clear();
    Material.clear();

    DXRelacher(pConstantBuffer);
    DXRelacher(pSampleState);

    DXRelacher(pEffet);
    DXRelacher(pVertexLayout);
    DXRelacher(pIndexBuffer);
    DXRelacher(pVertexBuffer);
  }

  void CelMeshRenderComponent::InitEffet()
  {
    LoaderSingleton::Get().CompileAndLoadEffectShader(
      "CelShading",
      &pSampleState,
      &pVertexLayout,
      MeshVertex::layout.data(),
      MeshVertex::layout.size(),
      sizeof(ShadersParams),
      &pConstantBuffer,
      &pEffet,
      &pTechnique,
      &pPasse1,
      &pPasse2);
  }

  void CelMeshRenderComponent::TransfertObjet(IChargeur& chargeur)
  {
    // 1. SOMMETS a) Créations des sommets dans un tableau temporaire

    unsigned int nombreSommets = 0;
    vector<MeshVertex> ts;
    MeshVertex s;
    vector<unsigned int> SommetDansMesh;
    vector<unsigned int> Indices;

    for ( unsigned int iSubmesh = 0; iSubmesh < chargeur.GetNombreSubmesh(); ++iSubmesh )
    {
      SommetDansMesh.push_back(ts.size());
      for ( unsigned int i = 0; i < chargeur.GetNombreSommetsSubmesh(iSubmesh); ++i )
      {
        s.position = chargeur.GetPosition(iSubmesh, i);
        s.normal = chargeur.GetNormale(iSubmesh, i);
        s.coordTex = chargeur.GetCoordTex(iSubmesh, i);

        ts.push_back(s);
      }
    }

    for ( unsigned int iSubmesh = 0; iSubmesh < chargeur.GetNombreSubmesh(); ++iSubmesh )
    {
      SubmeshIndex.push_back(Indices.size());
      for ( unsigned int i = 0; i < chargeur.GetNombrePolygonesSubmesh(iSubmesh); ++i )
      {
        for ( unsigned int j = 0; j < 3; ++j )
        {
          unsigned int index = chargeur.GetIndice(iSubmesh, i, j);
          Indices.push_back(SommetDansMesh[ iSubmesh ] + index);
        }
      }
    }
    SubmeshIndex.push_back(Indices.size());

    // 1. SOMMETS b) Création du vertex buffer et copie des sommets
    ID3D11Device* pD3DDevice = GameSingleton::Get().GetDispositif()->GetD3DDevice();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(MeshVertex)* ts.size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = ts.data();
    pVertexBuffer = NULL;

    DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer),
      DXE_CREATIONVERTEXBUFFER);

    // 2. INDEX - Création de l'index buffer et copie des indices
    //            Les indices étant habituellement des entiers, j'ai
    //            pris directement ceux du chargeur mais attention au
    //            format si vous avez autre chose que DXGI_FORMAT_R32_UINT
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned int)* Indices.size();
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = Indices.data();
    pIndexBuffer = NULL;

    DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
      DXE_CREATIONINDEXBUFFER);

    // 3. Les sous-objets
    NombreSubmesh = chargeur.GetNombreSubmesh();

    // 4. MATERIAUX
    // 4a) Créer un matériau de défaut en index 0
    //     Vous pourriez changer les valeurs, j'ai conservé
    //     celles du constructeur
    CMaterial m;

    Material.push_back(m);

    // 4b) Copie des matériaux dans la version locale
    for ( unsigned int i = 0; i < chargeur.GetNombreMaterial(); ++i )
    {
      CMaterial mat;

      chargeur.GetMaterial(i, mat);

      Material.push_back(mat);
    }

    // 4c) Trouver l'index du materiau pour chaque sous-ensemble (+1 car 0 est default)
    for ( unsigned int i = 0; i < chargeur.GetNombreSubmesh(); ++i )
    {
      SubmeshMaterialIndex.push_back(chargeur.GetMaterialIndex(i) + 1);
    }

    // 4d) Chargement des textures
    auto textureManager = Game<ITextureManager>();

    for ( unsigned int i = 0; i < Material.size(); ++i )
    {
      if ( Material[ i ].NomFichierTexture != "" )
      {
        Material[ i ].pTextureD3D = textureManager->GetNewTexture(Material[ i ].NomFichierTexture)->GetD3DTexture();
      }
    }
  }

  void CelMeshRenderComponent::Draw() const
  {
    // Obtenir le contexte
    ID3D11DeviceContext* pImmediateContext = GameSingleton::Get().GetDispositif()->GetImmediateContext();

    // Choisir la topologie des primitives
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // input layout des sommets
    pImmediateContext->IASetInputLayout(pVertexLayout);

    // Index buffer
    pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Vertex buffer
    UINT stride = sizeof(MeshVertex);
    UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

    // Initialiser et sélectionner les «constantes» de l'effet
    auto cam = Game<ICameraManager>()->GetMainCamera();
    ShadersParams sp;
    XMMATRIX viewProj = cam->GetMatViewProj();

    auto poseInterface = As<IPoseComponentInterface>();
    XMMATRIX matWorld = XMMatrixIdentity();
    if ( poseInterface )
    {
      matWorld = math::FromPxTransformToWorldMatrix(poseInterface->GetTransform());
    }

    sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
    sp.matWorld = XMMatrixTranspose(matWorld);
    sp.vLumiere = Game<IWorldManager>()->GetLightPosition();
    sp.vDEcl = light_value;

    // Le sampler state
    ID3DX11EffectSamplerVariable* variableSampler;
    variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, pSampleState);

    // Dessiner les sous-objets non-transparents
    for ( int i = 0; i < NombreSubmesh; ++i )
    {
      int indexStart = SubmeshIndex[ i ];
      int indexDrawAmount = SubmeshIndex[ i + 1 ] - SubmeshIndex[ i ];
      if ( indexDrawAmount )
      {
        sp.vDMat = XMLoadFloat4(&Material[ SubmeshMaterialIndex[ i ] ].Diffuse);

        // Activation de la texture ou non
        if ( Material[ SubmeshMaterialIndex[ i ] ].pTextureD3D != NULL )
        {
          ID3DX11EffectShaderResourceVariable* variableTexture;
          variableTexture =
            pEffet->GetVariableByName("textureEntree")->AsShaderResource();

          variableTexture->SetResource(Material[ SubmeshMaterialIndex[ i ] ].pTextureD3D);
          sp.bTex = 1;
        }
        else
        {
          sp.bTex = 0;
        }
        GameSingleton::Get().GetDispositif()->ActivateFrontCulling();
        // IMPORTANT pour ajuster les param.
        pPasse1->Apply(0, pImmediateContext);
        {
          // Nous n'avons qu'un seul CBuffer
          ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");
          pCB->SetConstantBuffer(pConstantBuffer);
          pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);
        }

        pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
        GameSingleton::Get().GetDispositif()->ActivateBackCulling();

        pPasse2->Apply(0, pImmediateContext);
        {
          // Nous n'avons qu'un seul CBuffer
          ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");
          pCB->SetConstantBuffer(pConstantBuffer);
          pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);
        }

        pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
      }
    }
  }

  void CelMeshRenderComponent::ChangeLighting(DirectX::XMVECTOR new_light)
  {
    light_value = new_light;
  }

  bool CelMeshRenderComponent::CanOptimizeLod() const
  {
    return lod_optimizability_;
  }
}