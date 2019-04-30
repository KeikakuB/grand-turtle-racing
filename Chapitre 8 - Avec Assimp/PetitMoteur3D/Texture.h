#pragma once
#include "dispositifD3D11.h"
#include <d3d11.h>
#include <boost/filesystem.hpp>
namespace PM3D
{
  class Texture
  {
  public:
    virtual ~Texture();
    Texture(const boost::filesystem::path& filename, CDispositifD3D11* pDispositif);
    const boost::filesystem::path& GetFilename() { return filename_; }
    ID3D11ShaderResourceView* GetD3DTexture() { return texture_; }
  private:
    boost::filesystem::path filename_;
    ID3D11ShaderResourceView* texture_;
  };
}