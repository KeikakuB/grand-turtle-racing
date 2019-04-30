#pragma once
#include "Meta.h"
#include "IRenderComponentInterface.h"
#include "GameObjectComponent.h"

#include "dispositifD3D11.h"
#include "d3dx11effect.h"
#include "chargeur.h"
#include "Material.h"

#include <vector>
#include <array>

namespace PM3D
{
  class MeshRenderComponent final :
    public GameObjectComponent, public IRenderComponentInterface
  {
  protected:
    class MeshVertex
    {
    public:
      MeshVertex() = default;
      MeshVertex(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT3 _normal,
        DirectX::XMFLOAT2 _coordTex = DirectX::XMFLOAT2(0, 0));

    public:
      static std::array<D3D11_INPUT_ELEMENT_DESC, 3> layout;

      DirectX::XMFLOAT3 position;
      DirectX::XMFLOAT3 normal;
      DirectX::XMFLOAT2 coordTex;
    };
  public:
    MeshRenderComponent(const std::string& id);
    ~MeshRenderComponent();
    void Draw() const override;
    bool CanOptimizeLod() const override;
    void SetLodOptimizability(bool opt) { lod_optimizability_ = opt; };
    void TransfertObjet(IChargeur& chargeur);
    void InitEffet();
  private:
    const ModelSettings& model_settings_;
    bool lod_optimizability_;
    struct ShadersParams
    {
      DirectX::XMMATRIX matWorldViewProj;	// la matrice totale
      DirectX::XMMATRIX matWorld;			// matrice de transformation dans le monde
      DirectX::XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
      DirectX::XMVECTOR vCamera; 			// la position de la caméra
      DirectX::XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
      DirectX::XMVECTOR vAMat; 			// la valeur ambiante du matériau
      DirectX::XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage
      DirectX::XMVECTOR vDMat; 			// la valeur diffuse du matériau
      DirectX::XMVECTOR vSEcl; 			// la valeur spéculaire de l'éclairage
      DirectX::XMVECTOR vSMat; 			// la valeur spéculaire du matériau
      float puissance;			// la puissance de spécularité
      int bTex;					// Texture ou materiau
      DirectX::XMFLOAT2 remplissage;
    };

    ID3D11Buffer* pVertexBuffer;
    ID3D11Buffer* pIndexBuffer;

    // Les sous-objets
    int NombreSubmesh;				    // Nombre de sous-objets dans le mesh
    std::vector<int> SubmeshMaterialIndex;	// Index des matériaux
    std::vector<int> SubmeshIndex;			// Index des sous-objets
    std::vector<CMaterial> Material;			// Vecteur des matériaux

                                    // Pour les effets et shaders
    ID3D11SamplerState* pSampleState;
    ID3D11Buffer* pConstantBuffer;
    ID3DX11Effect* pEffet;
    ID3DX11EffectTechnique* pTechnique;
    ID3DX11EffectPass* pPasse;
    ID3D11InputLayout* pVertexLayout;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(MeshRenderComponent);
  }
}