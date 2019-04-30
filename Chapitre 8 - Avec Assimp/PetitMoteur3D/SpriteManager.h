#pragma once
#include "Meta.h"
#include "d3dx11effect.h"
#include "dispositifD3D11.h"
#include "Texture.h"
#include "GameManagerComponent.h"
#include "ISpriteManager.h"
#include "GameObject.h"
#include "PoseComponent.h"

#include <vector>
#include <string>
#include <map>

#include <memory>

#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
  class CSommetSprite
  {
  public:
    CSommetSprite() {};
    CSommetSprite(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT2 _coordTex)
    {
      position = _position; coordTex = _coordTex;
    }

  public:
    static UINT numElements;
    static D3D11_INPUT_ELEMENT_DESC layout[];

    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 coordTex;
  };

  class SpriteManager : public GameManagerComponent, public ISpriteManager
  {
  public:
    SpriteManager();
    virtual ~SpriteManager();
    virtual	void Draw() override;

    virtual void AddSprite(SpriteGroup sg, std::string NomTexture, int _x, int _y, int _dx = 0, int _dy = 0) override;

    virtual void AddPanelSprite(SpriteGroup sg, std::string NomTexture, const physx::PxTransform& _transform, float _dx = 0.0f, float _dy = 0.0f) override;

    virtual void AddTextSprite(SpriteGroup sg, ID3D11ShaderResourceView* pTexture, int _x, int _y) override;

    virtual void EnableDrawFor(SpriteGroup sg) override;

    virtual void DisableDrawFor(SpriteGroup sg) override;

    virtual void ClearSpritesFor(SpriteGroup sg) override;

    virtual void RunTask() override;

  protected:
    class CSprite
    {
    public:
      ID3D11ShaderResourceView* pTextureD3D;
      DirectX::XMMATRIX matPosDim;
      bool bPanneau;
      CSprite()
      {
        bPanneau = false;
      }
      virtual ~CSprite() = default;
    };

    class CPanneau : public CSprite, public GameObject
    {
    public:
      DirectX::XMFLOAT2 dimension;
      CPanneau()
      {
        bPanneau = true;
      }
      void OnSpawn(const physx::PxTransform& pose) override
      {
        AddComponent<PoseComponent>(pose);
      }
    };

  protected:
    static CSommetSprite sommets[ 6 ];
    ID3D11Buffer* pVertexBuffer;

    ID3D11Buffer* pConstantBuffer;
    ID3DX11Effect* pEffet;
    ID3DX11EffectTechnique* pTechnique;
    ID3DX11EffectPass* pPasse;
    ID3D11InputLayout* pVertexLayout;

    ID3D11SamplerState* pSampleState;

    // Tous nos sprites
    std::map<SpriteGroup, std::vector<std::unique_ptr<CSprite>>> tabSprites;
    std::map<SpriteGroup, std::vector<CPanneau*>> panneauSprites;
    std::map<SpriteGroup, bool> isGroupEnabled;

  protected:
    virtual	void InitEffet();
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(SpriteManager);
  }
}
