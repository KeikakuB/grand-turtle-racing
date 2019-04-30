#pragma once
#include "Meta.h"

#include <string>

#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
  enum class SpriteGroup { kGenericGroup, kGuiInGameGroup, kGuiPauseMenuGroup, kLevelGroup, kSkyboxGroup, kGuiEndGameGroup, kGuiStartGroup, kGuiGameOverGroup };
  class ISpriteManager
  {
  public:
    virtual void AddSprite(SpriteGroup sg, std::string NomTexture, int _x, int _y, int _dx = 0, int _dy = 0) = 0;
    virtual void AddPanelSprite(SpriteGroup sg, std::string NomTexture, const physx::PxTransform& _transform, float _dx = 0.0f, float _dy = 0.0f) = 0;
    virtual void AddTextSprite(SpriteGroup sg, ID3D11ShaderResourceView* pTexture, int _x, int _y) = 0;
    virtual void EnableDrawFor(SpriteGroup sg) = 0;
    virtual void DisableDrawFor(SpriteGroup sg) = 0;
    virtual void ClearSpritesFor(SpriteGroup sg) = 0;
    virtual void Draw() = 0;
    virtual ~ISpriteManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(ISpriteManager);
  }
}
