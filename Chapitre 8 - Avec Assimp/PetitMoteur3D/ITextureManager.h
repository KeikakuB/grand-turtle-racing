#pragma once
#include "Meta.h"
#include "Texture.h"
#include <boost/filesystem.hpp>

namespace PM3D
{
  class ITextureManager
  {
  public:
    virtual Texture* const GetNewTexture(const boost::filesystem::path& filename) = 0;
    virtual Texture* const GetTexture(const boost::filesystem::path& filename) = 0;
    virtual ~ITextureManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(ITextureManager);
  }
}
