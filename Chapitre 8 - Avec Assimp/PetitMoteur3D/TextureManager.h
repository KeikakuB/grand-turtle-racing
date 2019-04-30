#pragma once
#include "Meta.h"
#include "ITextureManager.h"
#include "GameManagerComponent.h"
#include <vector>
namespace PM3D
{
  class TextureManager final : public GameManagerComponent, public ITextureManager
  {
  public:
    TextureManager();
    ~TextureManager();
    virtual Texture* const GetNewTexture(const boost::filesystem::path& filename) override;
    virtual Texture* const GetTexture(const boost::filesystem::path& filename) override;

    virtual void RunTask() override {}

  private:
    std::vector<Texture*> ListeTextures;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(TextureManager);
  }
}
