#include "stdafx.h"
#include "ConfigSingleton.h"

#include <boost/property_tree/json_parser.hpp>
#include <exception>
#include <iostream>
namespace bptree = boost::property_tree;
namespace bpath = boost::filesystem;

namespace PM3D
{
  ConfigSingleton::ConfigSingleton(const std::string& filename)
  {
    if ( !std::ifstream{ filename } )
    {
      throw no_such_file_exists{};
    }
    load(filename);
  }
  void ConfigSingleton::load(const std::string & filename)
  {
    // Create empty property tree object
    bptree::ptree tree;
    // Parse the XML into the property tree.
    bptree::read_json(filename, tree);

    // Use get_child to find the node containing the modules, and iterate over
    // its children. If the path cannot be resolved, get_child throws.
    for ( bptree::ptree::value_type &v : tree.get_child("shaders.vertex") )
    {
      auto& val = v.second;
      std::string id = val.get<std::string>("id");
      vertexShaders[ id ] = VertexShaderSettings{ val.get<std::string>("name"), val.get<std::string>("target"), ConvertToPath(val.get<std::string>("filename")) };
    }
    for ( bptree::ptree::value_type &v : tree.get_child("shaders.pixel") )
    {
      auto& val = v.second;
      std::string id = val.get<std::string>("id");
      pixelShaders[ id ] = PixelShaderSettings{ val.get<std::string>("name"), val.get<std::string>("target"), ConvertToPath(val.get<std::string>("filename")) };
    }
    for ( bptree::ptree::value_type &v : tree.get_child("shaders.effect") )
    {
      auto& val = v.second;
      std::string id = val.get<std::string>("id");
      effectShaders[ id ] = EffectShaderSettings{ val.get<std::string>("name"), val.get<std::string>("target"), ConvertToPath(val.get<std::string>("filename")) };
    }
    for ( bptree::ptree::value_type &v : tree.get_child("post_effect_techniques") )
    {
      auto& val = v.second;
      std::string id = val.get<std::string>("id");
      post_effect_techniques[ id ] = PostEffectTechniqueSettings{ id };
    }
    for ( bptree::ptree::value_type &v : tree.get_child("shader_configs") )
    {
      auto& val = v.second;
      std::string id = val.get<std::string>("id");
      ShaderSettings setting
      {
        id,
        val.get<float>("transparency")
      };
      materials[ id ] = std::move(setting);
    }
    for ( bptree::ptree::value_type &v : tree.get_child("models") )
    {
      auto& val = v.second;
      std::string id = val.get<std::string>("id");

      const ShaderSettings* mat;
      try
      {
        mat = &materials.at(val.get<std::string>("shader_config_ref"));
      }
      catch ( std::out_of_range& )
      {
        throw no_such_shader_config_exists{};
      }
      auto& texture = val.get_child("texture");
      ModelSettings setting
      {
        ConvertToPath(val.get<std::string>("filename")),
        ConvertToPath(val.get<std::string>("relative_path")),
        val.get<bool>("left_hand"),
        val.get<bool>("winding_order"),
        mat,
        TextureSettings{ ConvertToPath(texture.get<std::string>("filename"))}
      };
      models[ id ] = std::move(setting);
    }
  }
  DirectX::XMFLOAT4 ConfigSingleton::ParseFloat4(const bptree::ptree & tree) const
  {
    return DirectX::XMFLOAT4{ tree.get<float>("x"), tree.get<float>("y") , tree.get<float>("z") , tree.get<float>("w") };
  }
  boost::filesystem::path ConfigSingleton::ConvertToPath(const std::string str) const
  {
    return bpath::path{ str };
  }
}