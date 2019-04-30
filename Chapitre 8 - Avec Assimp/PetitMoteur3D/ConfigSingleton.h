#pragma once
#include "Singleton.h"
#include "SettingsStructs.h"
#include <boost/property_tree/ptree.hpp>

#include <string>
#include <map>

namespace PM3D
{
  class no_such_file_exists {};
  class no_such_model_exists {};
  class no_such_shader_config_exists {};
  class no_such_effect_shader_exists {};
  class no_such_pixel_shader_exists {};
  class no_such_vertex_shader_exists {};
  class no_such_post_effect_technique_exists {};

  class ConfigSingleton final : public Singleton<ConfigSingleton>
  {
  public:
    using size_type = size_t;
    friend class Singleton<ConfigSingleton>;
    template<typename T>
    size_type GetNumberOfSettings() const;
    template<>
    size_type GetNumberOfSettings<PostEffectTechniqueSettings>() const
    {
      return post_effect_techniques.size();
    }

    template<typename T>
    const T& FetchById(const std::string& id) const;
    template<>
    const VertexShaderSettings& FetchById(const std::string& id) const
    {
      try { return vertexShaders.at(id); }
      catch ( std::out_of_range& ) { throw no_such_vertex_shader_exists{}; }
    }
    template<>
    const PixelShaderSettings& FetchById(const std::string& id) const
    {
      try { return pixelShaders.at(id); }
      catch ( std::out_of_range& ) { throw no_such_pixel_shader_exists{}; }
    }
    template<>
    const EffectShaderSettings& FetchById(const std::string& id) const
    {
      try { return effectShaders.at(id); }
      catch ( std::out_of_range& ) { throw no_such_effect_shader_exists{}; }
    }
    template<>
    const ModelSettings& FetchById(const std::string& id) const
    {
      try { return models.at(id); }
      catch ( std::out_of_range& ) { throw no_such_model_exists{}; }
    }
    template<>
    const PostEffectTechniqueSettings& FetchById(const std::string& id) const
    {
      try { return post_effect_techniques.at(id); }
      catch ( std::out_of_range& ) { throw no_such_post_effect_technique_exists{}; }
    }
  private:
    ConfigSingleton(const std::string& filename);
    boost::filesystem::path ConfigSingleton::ConvertToPath(const std::string str) const;
    DirectX::XMFLOAT4 ParseFloat4(const boost::property_tree::ptree& tree) const;
    void load(const std::string &filename);
    std::map<std::string, VertexShaderSettings> vertexShaders;
    std::map<std::string, PixelShaderSettings> pixelShaders;
    std::map<std::string, EffectShaderSettings> effectShaders;
    std::map<std::string, ShaderSettings> materials;
    std::map<std::string, ModelSettings> models;
    std::map<std::string, PostEffectTechniqueSettings> post_effect_techniques;
  };
}
