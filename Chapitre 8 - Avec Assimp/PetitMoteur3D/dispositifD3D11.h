#pragma once

#include "dispositif.h"

namespace PM3D
{
  //
  //  Classe CDispositifD3D11
  //
  //  BUT : 	Classe permettant d'implanter un dispositif de rendu
  // 			Direct3D
  //
  class CDispositifD3D11 : public CDispositif
  {
  public:
    CDispositifD3D11(const CDS_MODE cdsMode, const HWND hWnd);

    virtual ~CDispositifD3D11();

    virtual void PresentSpecific();

    // Fonction d'accès aux membres protégés
    ID3D11Device* GetD3DDevice() { return pD3DDevice; }
    ID3D11DeviceContext*    GetImmediateContext() { return pImmediateContext; }
    IDXGISwapChain*         GetSwapChain() { return pSwapChain; }
    ID3D11RenderTargetView* GetRenderTargetView() { return pRenderTargetView; }
    ID3D11DepthStencilView* GetDepthStencilView() { return pDepthStencilView; }
    void CDispositifD3D11::ActiverMelangeAlpha()
    {
      float facteur[ 4 ] = { 0.0f,0.0f,0.0f,0.0f };
      // Activer le mélange - alpha blending.
      pImmediateContext->OMSetBlendState(alphaBlendEnable, facteur, 0xffffffff);
    }
    void CDispositifD3D11::DesactiverMelangeAlpha()
    {
      float facteur[ 4 ] = { 0.0f,0.0f,0.0f,0.0f };
      // Désactiver le mélange - alpha blending.
      pImmediateContext->OMSetBlendState(alphaBlendDisable, facteur, 0xffffffff);
    }
    void CDispositifD3D11::SetRenderTargetView(ID3D11RenderTargetView* pRenderTargetView_in,
      ID3D11DepthStencilView* pDepthStencilView_in);

    void ActivateZBuffer();
    void DeactivateZBuffer();
    void ActivateBackCulling();
    void ActivateFrontCulling();
    void DeactivateCulling();

  protected:
    CDispositifD3D11() = default;
    void InitDepthBuffer();
    // Initialiser les états de mélange (blending states)
    void InitBlendStates();

    // Pour le mélange alpha (transparence)
    ID3D11BlendState* alphaBlendEnable;
    ID3D11BlendState* alphaBlendDisable;

    ID3D11Device* pD3DDevice;
    ID3D11DeviceContext*    pImmediateContext;
    IDXGISwapChain*         pSwapChain;
    ID3D11RenderTargetView* pRenderTargetView;

    // Pour le tampon de profondeur
    ID3D11Texture2D* pDepthTexture;
    ID3D11DepthStencilView* pDepthStencilView;

    // Variables d'état
    ID3D11RasterizerState* mSolidCullBackRS;
    ID3D11RasterizerState* mSolidCullFrontRS;
    ID3D11RasterizerState* mSolidCullNoneRS;
    ID3D11DepthStencilState* mZBufferInactif;
    ID3D11DepthStencilState* mZBufferActif;
  };
}
