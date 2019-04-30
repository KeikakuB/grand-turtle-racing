#include "stdafx.h"
#include "SpriteManager.h"
#include "resource.h"
#include "MoteurWindows.h"
#include "GameSingleton.h"
#include "ITextureManager.h"
#include "util.h"
#include "Math.h"
#include "IPoseComponentInterface.h"

using namespace UtilitairesDX;
using namespace DirectX;

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(SpriteManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<SpriteManager>() noexcept { return{ GetInterfaceId<ISpriteManager>() }; }
  }
  struct ShadersParams
  {
    XMMATRIX matWVP;	// la matrice totale
  };

  // Definir l'organisation de notre sommet
  D3D11_INPUT_ELEMENT_DESC CSommetSprite::layout[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
  };

  UINT CSommetSprite::numElements = ARRAYSIZE(layout);

  CSommetSprite SpriteManager::sommets[ 6 ] = {
          CSommetSprite(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
          CSommetSprite(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
          CSommetSprite(XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
          CSommetSprite(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
          CSommetSprite(XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
          CSommetSprite(XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f))
  };

  SpriteManager::SpriteManager()
  {
    pVertexBuffer = 0;
    pConstantBuffer = 0;
    pEffet = 0;
    pTechnique = 0;
    pPasse = 0;
    pVertexLayout = 0;
    pSampleState = 0;

    // Création du vertex buffer et copie des sommets
    ID3D11Device* pD3DDevice = GameSingleton::Get().GetDispositif()->GetD3DDevice();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CSommetSprite) * 6;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = sommets;
    pVertexBuffer = NULL;

    DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer),
      DXE_CREATIONVERTEXBUFFER);

    // Initialisation de l'effet
    InitEffet();
  }

  SpriteManager ::~SpriteManager()
  {
    DXRelacher(pConstantBuffer);
    DXRelacher(pSampleState);

    DXRelacher(pEffet);
    DXRelacher(pVertexLayout);
    DXRelacher(pVertexBuffer);
  }

  void SpriteManager::InitEffet()
  {
    // Compilation et chargement du vertex shader
    ID3D11Device* pD3DDevice = GameSingleton::Get().GetDispositif()->GetD3DDevice();

    // Création d'un tampon pour les constantes de l'effet
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ShadersParams);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

    // Pour l'effet
    ID3DBlob* pFXBlob = NULL;

    DXEssayer(D3DCompileFromFile(L".\\data\\sprite1.fx", 0, 0, 0,
      "fx_5_0", 0, 0,
      &pFXBlob, 0),
      DXE_ERREURCREATION_FX);

    D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(),
      pFXBlob->GetBufferSize(), 0, pD3DDevice, &pEffet);

    pFXBlob->Release();

    pTechnique = pEffet->GetTechniqueByIndex(0);
    pPasse = pTechnique->GetPassByIndex(0);

    // Créer l'organisation des sommets pour le VS de notre effet
    D3DX11_PASS_SHADER_DESC effectVSDesc;
    pPasse->GetVertexShaderDesc(&effectVSDesc);

    D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
    effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex,
      &effectVSDesc2);

    const void *vsCodePtr = effectVSDesc2.pBytecode;
    unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

    pVertexLayout = NULL;
    DXEssayer(pD3DDevice->CreateInputLayout(
      CSommetSprite::layout,
      CSommetSprite::numElements,
      vsCodePtr,
      vsCodeLen,
      &pVertexLayout),
      DXE_CREATIONLAYOUT);

    // Initialisation des paramètres de sampling de la texture
    D3D11_SAMPLER_DESC samplerDesc;

    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 4;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[ 0 ] = 0;
    samplerDesc.BorderColor[ 1 ] = 0;
    samplerDesc.BorderColor[ 2 ] = 0;
    samplerDesc.BorderColor[ 3 ] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Création de l'état de sampling
    pD3DDevice->CreateSamplerState(&samplerDesc, &pSampleState);
  }

  void SpriteManager::ClearSpritesFor(SpriteGroup sg)
  {
    tabSprites.erase(sg);
    panneauSprites.erase(sg);
  }

  void SpriteManager::RunTask()
  {
    for ( auto x : panneauSprites )
    {
      for ( auto pan : x.second )
      {
        auto cam = Game<ICameraManager>()->GetMainCamera();
        // Position en coordonnées du monde
        XMMATRIX viewProj = cam->GetMatViewProj();

        auto pose = pan->As<IPoseComponentInterface>()->GetTransform();
        physx::PxVec3 p = pose.p;

        DirectX::XMMATRIX matWorld = XMMatrixIdentity();
        DirectX::XMMATRIX dx_rot = math::FromPxQuatToMatrix(pose.q);
        DirectX::XMMATRIX offset_translation = XMMatrixTranslation(pan->dimension.x / 2, pan->dimension.y / 2, 0.0f);
        DirectX::XMMATRIX offset_translation_inverse = XMMatrixTranslation(-( pan->dimension.x / 2 ), -( pan->dimension.y / 2 ), 0.0f);
        matWorld *= XMMatrixScaling(pan->dimension.x, pan->dimension.y, 1.0f);
        matWorld *= dx_rot;
        matWorld *= XMMatrixTranslation(p.x, p.y, p.z);
        matWorld *= viewProj;

        pan->matPosDim = matWorld;
      }
    }
  }

  void SpriteManager::EnableDrawFor(SpriteGroup sg)
  {
    isGroupEnabled[ sg ] = true;
  }

  void SpriteManager::DisableDrawFor(SpriteGroup sg)
  {
    isGroupEnabled[ sg ] = false;
  }

  void SpriteManager::Draw()
  {
    // Obtenir le contexte
    ID3D11DeviceContext* pImmediateContext =
      GameSingleton::Get().GetDispositif()->GetImmediateContext();

    // Choisir la topologie des primitives
    pImmediateContext->IASetPrimitiveTopology(
      D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Source des sommets
    UINT stride = sizeof(CSommetSprite);
    UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride,
      &offset);

    // input layout des sommets
    pImmediateContext->IASetInputLayout(pVertexLayout);

    // Le sampler state
    ID3DX11EffectSamplerVariable* variableSampler;
    variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, pSampleState);

    ID3DX11EffectConstantBuffer* pCB =
      pEffet->GetConstantBufferByName("param");
    ID3DX11EffectShaderResourceVariable* variableTexture;
    variableTexture =
      pEffet->GetVariableByName("textureEntree")->AsShaderResource();

    GameSingleton::Get().GetDispositif()->ActiverMelangeAlpha();

    for ( auto& x : tabSprites )
    {
      if ( isGroupEnabled[ x.first ] )
      {
        for ( auto& sprite : x.second )
        {
          // Initialiser et sélectionner les «constantes» de l'effet
          ShadersParams sp;
          sp.matWVP = XMMatrixTranspose(sprite->matPosDim);
          pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL,
            &sp, 0, 0);

          pCB->SetConstantBuffer(pConstantBuffer);

          // Activation de la texture
          variableTexture->SetResource(sprite->pTextureD3D);

          pPasse->Apply(0, pImmediateContext);

          // **** Rendu de l'objet
          pImmediateContext->Draw(6, 0);
        }
      }
    }

    GameSingleton::Get().GetDispositif()->DesactiverMelangeAlpha();
  }

  void SpriteManager::AddSprite(SpriteGroup sg,
    std::string NomTexture,
    int _x, int _y,
    int _dx, int _dy)
  {
    float x, y, dx, dy;
    float posX, posY;
    float facteurX, facteurY;

    auto texture_manager = Game<ITextureManager>();

    std::unique_ptr<CSprite> pSprite = std::make_unique<CSprite>();
    pSprite->pTextureD3D =
      texture_manager->GetNewTexture(NomTexture)->GetD3DTexture();

    // Obtenir les dimensions de la texture si _dx et _dy sont à 0;
    if ( _dx == 0 && _dy == 0 )
    {
      ID3D11Resource* pResource;
      ID3D11Texture2D *pTextureInterface = 0;
      pSprite->pTextureD3D->GetResource(&pResource);
      pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
      D3D11_TEXTURE2D_DESC desc;
      pTextureInterface->GetDesc(&desc);

      dx = float(desc.Width);
      dy = float(desc.Height);
    }
    else
    {
      dx = float(_dx);
      dy = float(_dy);
    }

    // Dimension en facteur
    facteurX = dx*2.0f / GameSingleton::Get().GetDispositif()->GetLargeur();
    facteurY = dy*2.0f / GameSingleton::Get().GetDispositif()->GetHauteur();

    // Position en coordonnées logiques
    // 0,0 pixel = -1,1
    x = float(_x);
    y = float(_y);

    posX = x*2.0f / GameSingleton::Get().GetDispositif()->GetLargeur() - 1.0f;
    posY = 1.0f - y*2.0f / GameSingleton::Get().GetDispositif()->GetHauteur();

    pSprite->matPosDim = XMMatrixScaling(facteurX, facteurY, 1.0f) *
      XMMatrixTranslation(posX, posY, 0.0f);

    if ( tabSprites.find(sg) == tabSprites.end() )
    {
      tabSprites[ sg ] = std::vector<std::unique_ptr<CSprite>>{};
    }
    tabSprites[ sg ].push_back(std::move(pSprite));
  }

  void SpriteManager::AddPanelSprite(
    SpriteGroup sg,
    std::string NomTexture,
    const physx::PxTransform& _transform,
    float _dx, float _dy)
  {
    auto texture_manager = Game<ITextureManager>();

    std::unique_ptr<CPanneau> pPanneau = std::make_unique<CPanneau>();
    pPanneau->OnSpawn(_transform);
    pPanneau->pTextureD3D =
      texture_manager->GetNewTexture(NomTexture)->GetD3DTexture();

    // Obtenir la dimension de la texture si _dx et _dy sont à 0;
    if ( _dx == 0.0f && _dy == 0.0f )
    {
      ID3D11Resource* pResource;
      ID3D11Texture2D *pTextureInterface = 0;
      pPanneau->pTextureD3D->GetResource(&pResource);
      pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
      D3D11_TEXTURE2D_DESC desc;
      pTextureInterface->GetDesc(&desc);

      pPanneau->dimension.x = float(desc.Width);
      pPanneau->dimension.y = float(desc.Height);

      // Dimension en facteur
      pPanneau->dimension.x = pPanneau->dimension.x*2.0f / GameSingleton::Get().GetDispositif()->GetLargeur();
      pPanneau->dimension.y = pPanneau->dimension.y*2.0f / GameSingleton::Get().GetDispositif()->GetHauteur();
    }
    else
    {
      pPanneau->dimension.x = float(_dx);
      pPanneau->dimension.y = float(_dy);
    }

    if ( panneauSprites.find(sg) == panneauSprites.end() )
    {
      panneauSprites[ sg ] = std::vector<CPanneau*>{};
    }
    panneauSprites[ sg ].push_back(pPanneau.get());

    if ( tabSprites.find(sg) == tabSprites.end() )
    {
      tabSprites[ sg ] = std::vector<std::unique_ptr<CSprite>>{};
    }
    tabSprites[ sg ].push_back(std::move(pPanneau));
  }

  void SpriteManager::AddTextSprite(
    SpriteGroup sg,
    ID3D11ShaderResourceView* pTexture, int _x, int _y)
  {
    std::unique_ptr<CSprite> pSprite = std::make_unique<CSprite>();
    pSprite->pTextureD3D = pTexture;

    // Obtenir la dimension de la texture;
    ID3D11Resource* pResource;
    ID3D11Texture2D *pTextureInterface = 0;
    pSprite->pTextureD3D->GetResource(&pResource);
    pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
    D3D11_TEXTURE2D_DESC desc;
    pTextureInterface->GetDesc(&desc);

    float dx = float(desc.Width);
    float dy = float(desc.Height);

    // Dimension en facteur
    float facteurX = dx*2.0f / GameSingleton::Get().GetDispositif()->GetLargeur();
    float facteurY = dy*2.0f / GameSingleton::Get().GetDispositif()->GetHauteur();

    // Position en coordonnées logiques
    // 0,0 pixel = -1,1
    float x = float(_x);
    float y = float(_y);

    float posX = x*2.0f / GameSingleton::Get().GetDispositif()->GetLargeur() - 1.0f;
    float posY = 1.0f - y*2.0f / GameSingleton::Get().GetDispositif()->GetHauteur();

    pSprite->matPosDim = XMMatrixScaling(facteurX, facteurY, 1.0f) *
      XMMatrixTranslation(posX, posY, 0.0f);

    if ( tabSprites.find(sg) == tabSprites.end() )
    {
      tabSprites[ sg ] = std::vector<std::unique_ptr<CSprite>>{};
    }
    tabSprites[ sg ].push_back(std::move(pSprite));
  }
}