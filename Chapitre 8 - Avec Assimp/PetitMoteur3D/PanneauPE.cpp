#include "stdafx.h"
#include "PanneauPE.h"
#include "util.h"
#include "resource.h"
#include "GameSingleton.h"
#include "IGameLogicManager.h"
using namespace UtilitairesDX;
using namespace DirectX;

namespace PM3D
{
  struct ShadersParams
  {
    int screen_width;
    int screen_height;
    float RadialBlur_distance;
    int GradualBlur_starting_offset;
    float time;
    DirectX::XMFLOAT3 remplissage;
  };

  // Definir l'organisation de notre sommet
  D3D11_INPUT_ELEMENT_DESC CSommetPanneauPE::layout[] =
  {
    {"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA, 0 },
    {"TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,    0,12,D3D11_INPUT_PER_VERTEX_DATA, 0}
  };

  UINT CSommetPanneauPE::numElements = ARRAYSIZE(layout);

  CSommetPanneauPE CPanneauPE::sommets[ 6 ] = {
            CSommetPanneauPE(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
            CSommetPanneauPE(XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
            CSommetPanneauPE(XMFLOAT3(1.0f,  1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
            CSommetPanneauPE(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
            CSommetPanneauPE(XMFLOAT3(1.0f,  1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
            CSommetPanneauPE(XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)) };

  CPanneauPE::~CPanneauPE(void)
  {
    DXRelacher(pResourceView);
    DXRelacher(pRenderTargetView);
    DXRelacher(pTextureScene);
    DXRelacher(pDepthStencilView);
    DXRelacher(pDepthTexture);

    DXRelacher(pSampleState);

    DXRelacher(pEffet);
    DXRelacher(pVertexLayout);
    DXRelacher(pVertexBuffer);
  }

  //   FONCTION : CPanneauPE, constructeur paramètré
  //   BUT :	Constructeur d'une classe de PanneauPE
  //   PARAMÈTRE:
  //		pDispositif: pointeur sur notre objet dispositif
  CPanneauPE::CPanneauPE(int nombre_de_techniques)
    : nombre_de_techniques_(nombre_de_techniques)
  {
    pVertexBuffer = 0;
    pEffet = 0;
    pTechnique = 0;
    pPasse = 0;
    pSampleState = 0;

    // Création du vertex buffer et copie des sommets
    ID3D11Device* pD3DDevice = GameSingleton::Get().GetDispositif()->GetD3DDevice();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CSommetPanneauPE) * 6;
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

  void CPanneauPE::InitEffet()
  {
    // Compilation et chargement du vertex shader
    ID3D11Device* pD3DDevice = GameSingleton::Get().GetDispositif()->GetD3DDevice();

    // Pour l'effet
    ID3DBlob* pFXBlob = NULL;
    ID3DBlob* pErrBlob = NULL;

    DXEssayer(
      D3DCompileFromFile(L".//data//PostEffect.fx", 0, 0, 0,
        "fx_5_0", 0, 0,
        &pFXBlob, &pErrBlob),
      DXE_ERREURCREATION_FX);

    D3DX11CreateEffectFromMemory(
      pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &pEffet);

    pFXBlob->Release();
    // Créer l'organisation des sommets pour le VS de notre effet
    D3DX11_PASS_SHADER_DESC effectVSDesc;
    D3DX11_EFFECT_SHADER_DESC effectVSDesc2;

    pTechnique = pEffet->GetTechniqueByIndex(0);
    pPasse = pTechnique->GetPassByIndex(0);

    pPasse->GetVertexShaderDesc(&effectVSDesc);

    effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

    const void *vsCodePtr = effectVSDesc2.pBytecode;
    unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

    pVertexLayout = NULL;
    DXEssayer(pD3DDevice->CreateInputLayout(CSommetPanneauPE::layout,
      CSommetPanneauPE::numElements,
      vsCodePtr,
      vsCodeLen,
      &pVertexLayout),
      DXE_CREATIONLAYOUT);

    // Initialisation des paramètres de sampling de la texture
    // Pas nécessaire d'être compliqué puisque l'affichage sera
    // en 1 pour 1 et à plat
    D3D11_SAMPLER_DESC samplerDesc;

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 0;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[ 0 ] = 0;
    samplerDesc.BorderColor[ 1 ] = 0;
    samplerDesc.BorderColor[ 2 ] = 0;
    samplerDesc.BorderColor[ 3 ] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Création de l'état de sampling
    pD3DDevice->CreateSamplerState(&samplerDesc, &pSampleState);

    // *********************** POUR LE POST EFFECT **************************
    D3D11_TEXTURE2D_DESC textureDesc;
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

    // Description de la texture
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    // Cette texture sera utilisée comme cible de rendu et
    // comme ressource de shader
    textureDesc.Width = GameSingleton::Get().GetDispositif()->GetLargeur();
    textureDesc.Height = GameSingleton::Get().GetDispositif()->GetHauteur();;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    // Création de la texture
    pD3DDevice->CreateTexture2D(&textureDesc, NULL, &pTextureScene);

    // VUE - Cible de rendu
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    // Création de la vue.
    pD3DDevice->CreateRenderTargetView(pTextureScene,
      &renderTargetViewDesc,
      &pRenderTargetView);

    // VUE – Ressource de shader
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    // Création de la vue.
    pD3DDevice->CreateShaderResourceView(pTextureScene,
      &shaderResourceViewDesc,
      &pResourceView);

    // Au tour du tampon de profondeur
    D3D11_TEXTURE2D_DESC depthTextureDesc;
    ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
    depthTextureDesc.Width = GameSingleton::Get().GetDispositif()->GetLargeur();
    depthTextureDesc.Height = GameSingleton::Get().GetDispositif()->GetHauteur();
    depthTextureDesc.MipLevels = 1;
    depthTextureDesc.ArraySize = 1;
    depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthTextureDesc.SampleDesc.Count = 1;
    depthTextureDesc.SampleDesc.Quality = 0;
    depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthTextureDesc.CPUAccessFlags = 0;
    depthTextureDesc.MiscFlags = 0;

    DXEssayer(pD3DDevice->CreateTexture2D(&depthTextureDesc, NULL, &pDepthTexture),
      DXE_ERREURCREATIONTEXTURE);

    // Création de la vue du tampon de profondeur (ou de stencil)
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
    ZeroMemory(&descDSView, sizeof(descDSView));
    descDSView.Format = depthTextureDesc.Format;
    descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSView.Texture2D.MipSlice = 0;
    DXEssayer(pD3DDevice->CreateDepthStencilView(pDepthTexture, &descDSView,
      &pDepthStencilView),
      DXE_ERREURCREATIONDEPTHSTENCILTARGET);
  }

  void CPanneauPE::DebutPostEffect()
  {
    // Prendre en note l'ancienne surface de rendu
    pOldRenderTargetView = GameSingleton::Get().GetDispositif()->GetRenderTargetView();

    // Prendre en note l'ancienne surface de tampon Z
    pOldDepthStencilView = GameSingleton::Get().GetDispositif()->GetDepthStencilView();

    // Utiliser la texture comme surface de rendu et le tampon de profondeur
    // associé
    GameSingleton::Get().GetDispositif()->SetRenderTargetView(pRenderTargetView, pDepthStencilView);
  }

  void CPanneauPE::FinPostEffect()
  {
    // Restaurer l'ancienne surface de rendu et le tampon de profondeur
    // associé
    GameSingleton::Get().GetDispositif()->SetRenderTargetView(pOldRenderTargetView,
      pOldDepthStencilView);
  }

  void CPanneauPE::SetCurrentTimeParam(float param)
  {
    current_time_param_ = param;
  }

  void CPanneauPE::Draw()
  {
    auto logic_manager = Game<IGameLogicManager>();
    // Obtenir le contexte
    ID3D11DeviceContext* pImmediateContext = GameSingleton::Get().GetDispositif()->GetImmediateContext();

    // Choisir la topologie des primitives
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Source des sommets
    UINT stride = sizeof(CSommetPanneauPE);
    UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

    // Choix de la technique
    pTechnique = pEffet->GetTechniqueByName(current_technique_name_.c_str());
    pPasse = pTechnique->GetPassByIndex(0);

    // input layout des sommets
    pImmediateContext->IASetInputLayout(pVertexLayout);

    // Le sampler state
    ID3DX11EffectSamplerVariable* variableSampler;
    variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, pSampleState);

    ID3DX11EffectScalarVariable* screen_width;
    screen_width = pEffet->GetVariableByName("screen_width")->AsScalar();
    screen_width->SetInt(GameSingleton::Get().GetDispositif()->GetLargeur());

    ID3DX11EffectScalarVariable* screen_height;
    screen_height = pEffet->GetVariableByName("screen_height")->AsScalar();
    screen_height->SetInt(GameSingleton::Get().GetDispositif()->GetHauteur());

    ID3DX11EffectScalarVariable* time;
    time = pEffet->GetVariableByName("time")->AsScalar();
    time->SetFloat(current_time_param_);

    ID3DX11EffectScalarVariable* distance;
    distance = pEffet->GetVariableByName("RadialBlur_distance")->AsScalar();
    distance->SetFloat(0.0005f * logic_manager->GetTurtleVelocity());

    ID3DX11EffectScalarVariable* starting_offset;
    starting_offset = pEffet->GetVariableByName("GradualBlur_starting_offset")->AsScalar();
    starting_offset->SetInt(10);

    ID3DX11EffectShaderResourceVariable* variableTexture;
    variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();

    // Activation de la texture
    variableTexture->SetResource(pResourceView);

    pPasse->Apply(0, pImmediateContext);

    // **** Rendu de l'objet
    pImmediateContext->Draw(6, 0);
  }
}