#include "stdafx.h"
#include "TextureManager.h"
#include "GameSingleton.h"

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(TextureManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<TextureManager>() noexcept { return{ GetInterfaceId<ITextureManager>() }; }
  }
  TextureManager::TextureManager() = default;

  TextureManager::~TextureManager()
  {
    // détruire les objets
    std::vector<Texture*>::iterator It;

    for ( It = ListeTextures.begin(); It != ListeTextures.end(); It++ )
    {
      delete ( *It );
    }

    ListeTextures.clear();
  };

  Texture* const TextureManager::GetNewTexture(const boost::filesystem::path& filename)
  {
    Texture* pTexture;

    // On vérifie si la texture est déjà dans notre liste
    pTexture = GetTexture(filename);

    // Si non, on la crée
    if ( !pTexture )
    {
      pTexture = new Texture(filename, GameSingleton::Get().GetDispositif());

      // Puis, il est ajouté à la scène
      ListeTextures.push_back(pTexture);
    }

    return pTexture;
  }

  Texture* const TextureManager::GetTexture(const boost::filesystem::path& filename)
  {
    for ( Texture* text : ListeTextures )
    {
      if ( text->GetFilename() == filename )
      {
        return text;
      }
    }
    return nullptr;
  }
}