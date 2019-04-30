#pragma once
#include <string>
#include <boost/filesystem.hpp>

namespace PM3D
{
  struct VertexShaderSettings final
  {
    std::string name;
    std::string target;
    boost::filesystem::path filename;
  };
  struct PixelShaderSettings final
  {
    std::string name;
    std::string target;
    boost::filesystem::path filename;
  };
  struct EffectShaderSettings final
  {
    std::string name;
    std::string target;
    boost::filesystem::path filename;
  };
  struct ShaderSettings final
  {
    std::string id;
    float transparency;
  };
  struct TextureSettings final
  {
    boost::filesystem::path filename;
  };
  struct ModelSettings final
  {
    boost::filesystem::path filename;
    boost::filesystem::path relative_path;
    bool left_hand;
    bool winding_order;
    const ShaderSettings* shader_config;
    TextureSettings texture;
  };
  struct PostEffectTechniqueSettings final
  {
    std::string id;
  };
}
