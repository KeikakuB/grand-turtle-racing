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
  const unsigned int index_skybox[ 36 ] = {
    0,1,2,			// devant
    0,2,3,			// devant
    5,6,7,			// arri�re
    5,7,4,			// arri�re
    8,9,10,			// dessous
    8,10,11,		// dessous
    13,14,15,		// dessus
    13,15,12,		// dessus
    19,16,17,		// gauche
    19,17,18,		// gauche
    20,21,22,		// droite
    20,22,23		// droite
  };
  class SkyboxRenderComponent final :
    public GameObjectComponent, public IRenderComponentInterface
  {
  public:
    SkyboxRenderComponent(const boost::filesystem::path& filename);
    ~SkyboxRenderComponent();
    void Draw() const override;
    bool CanOptimizeLod() const override;
  private:
    SkyboxRenderComponent(const boost::filesystem::path& filename, float dx, float dy, float dz);
    void InitEffet();

    ID3D11Buffer* pVertexBuffer;
    ID3D11Buffer* pIndexBuffer;

    // D�finitions des valeurs d'animation
    ID3D11Buffer* pConstantBuffer;

    // Pour les effets
    ID3DX11Effect* pEffet;
    ID3DX11EffectTechnique* pTechnique;
    ID3DX11EffectPass* pPasse;
    ID3D11InputLayout* pVertexLayout;

    ID3D11ShaderResourceView* pTextureD3DFront;
    ID3D11ShaderResourceView* pTextureD3DBack;
    ID3D11ShaderResourceView* pTextureD3DLeft;
    ID3D11ShaderResourceView* pTextureD3DRight;
    ID3D11ShaderResourceView* pTextureD3DUp;
    ID3D11ShaderResourceView* pTextureD3DDown;
    ID3D11SamplerState* pSampleState;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(SkyboxRenderComponent);
  }
}
