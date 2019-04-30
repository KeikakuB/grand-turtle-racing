#include "StdAfx.h"
#include "strsafe.h"
#include "dispositifD3D11.h"
#include "Texture.h"
#include "resource.h"
#include "util.h"
#include "DDSTextureLoader.h"

using namespace UtilitairesDX;
using namespace DirectX;

namespace PM3D
{
  Texture::~Texture()
  {
    DXRelacher(texture_);
  }

  Texture::Texture(const boost::filesystem::path& filename, CDispositifD3D11* pDispositif)
    : filename_{ filename }, texture_{ nullptr }
  {
    ID3D11Device* device = pDispositif->GetD3DDevice();
    // Charger la texture en ressource
    DXEssayer(CreateDDSTextureFromFile(device,
      filename_.c_str(),
      nullptr,
      &texture_), DXE_FICHIERTEXTUREINTROUVABLE);
  }
}