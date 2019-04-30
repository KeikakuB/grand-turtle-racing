#pragma once
#include "Meta.h"
#include "IRenderComponentInterface.h"
#include "GameObjectComponent.h"

namespace PM3D
{
  const unsigned int index_bloc[ 36 ] = {
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
  class BlockRenderComponent final :
    public GameObjectComponent, public IRenderComponentInterface
  {
  public:
    BlockRenderComponent();
    BlockRenderComponent(float dx, float dy, float dz);
    ~BlockRenderComponent();
    void Draw() const override;
    bool CanOptimizeLod() const override;
    void SetLodOptimizability(bool opt) { lod_optimizability_ = opt; };
  private:
    bool lod_optimizability_;
    void InitShaders();
    ID3D11Buffer* pVertexBuffer;
    ID3D11Buffer* pIndexBuffer;

    ID3D11VertexShader*  pVertexShader;
    ID3D11PixelShader*  pPixelShader;
    ID3D11InputLayout* pVertexLayout;

    // Définitions des valeurs d'animation
    ID3D11Buffer* pConstantBuffer;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(BlockRenderComponent);
  }
}
