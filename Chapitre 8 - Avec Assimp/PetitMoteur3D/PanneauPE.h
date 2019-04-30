#pragma once
#include "dispositifD3D11.h"
#include "d3dx11effect.h"

#include <vector>

namespace PM3D
{
  class CSommetPanneauPE
  {
  public:
    CSommetPanneauPE() {};
    CSommetPanneauPE(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT2 _coordTex)
    {
      position = _position; coordTex = _coordTex;
    }

  public:
    static UINT numElements;
    static D3D11_INPUT_ELEMENT_DESC layout[];

    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 coordTex;
  };

  //   Classe : CPanneauPE
  //   BUT : 	Classe pour les post-effects
  class CPanneauPE
  {
  public:
    CPanneauPE(int nombre_de_techniques);
    ~CPanneauPE();

    void SetTechniqueByName(const std::string& name)
    {
      current_technique_name_ = name;
    }
    void Draw();
    void DebutPostEffect();
    void FinPostEffect();
    void SetCurrentTimeParam(float param);

  private:
    void InitEffet();
    int nombre_de_techniques_;
    float current_time_param_ = 0.0f;
    std::string current_technique_name_;
    static CSommetPanneauPE sommets[ 6 ];
    ID3D11Buffer* pVertexBuffer;

    // Pour les effets
    ID3DX11Effect* pEffet;
    ID3DX11EffectTechnique* pTechnique;
    ID3DX11EffectPass* pPasse;

    ID3D11InputLayout* pVertexLayout;

    ID3D11SamplerState* pSampleState;

    // Texture de rendu pour effets
    ID3D11Texture2D* pTextureScene;
    ID3D11RenderTargetView* pRenderTargetView;
    ID3D11ShaderResourceView* pResourceView;
    ID3D11Texture2D* pDepthTexture;
    ID3D11DepthStencilView* pDepthStencilView;

    ID3D11RenderTargetView* pOldRenderTargetView;
    ID3D11DepthStencilView* pOldDepthStencilView;
  };
}
