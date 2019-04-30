#pragma once
#include "Meta.h"
#include "IRenderComponentInterface.h"
#include "GameObjectComponent.h"
#include "Texture.h"
#include "dispositifD3D11.h"
#include "d3dx11effect.h"
#include "chargeur.h"

namespace PM3D
{
  const unsigned int index_billboard[ 36 ] = {
    2,1,0,			// devant
    3,2,0,		// devant
    7,6,5,		// arrière
    4,7,5,			// arrière
    10,9,8,			// dessous
    11,10,8,		// dessous
    15,14,13,		// dessus
    12,15,13,		// dessus
    17,16,19,		// gauche
    18,17,19,		// gauche
    22,21,20,		// droite
    23,22,20		// droite
  };
  class BillboardRenderComponent final :
    public GameObjectComponent, public IRenderComponentInterface
  {
  public:
    BillboardRenderComponent();
    BillboardRenderComponent(const boost::filesystem::path& filename, float dx, float dy, float dz);
    ~BillboardRenderComponent();
    void Draw() const override;
    bool CanOptimizeLod() const override;
    void SetLodOptimizability(bool opt) { lod_optimizability_ = opt; };
    void SetTexture(Texture* pTexture);
  private:
    bool lod_optimizability_;
    void InitEffet();

    ID3D11Buffer* pVertexBuffer;
    ID3D11Buffer* pIndexBuffer;

    // Définitions des valeurs d'animation
    ID3D11Buffer* pConstantBuffer;

    // Pour les effets
    ID3DX11Effect* pEffet;
    ID3DX11EffectTechnique* pTechnique;
    ID3DX11EffectPass* pPasse;
    ID3D11InputLayout* pVertexLayout;

    ID3D11ShaderResourceView* pTextureD3D;
    ID3D11SamplerState* pSampleState;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(BillboardRenderComponent);
  }
}
