#pragma once
#include <string>
namespace PM3D
{
  namespace ids
  {
    namespace shaders_vertex
    {
      extern const std::string kMiniPhongVS;
    }
    namespace shaders_pixel
    {
      extern const std::string kMiniPhongPS;
    }
    namespace shaders_effect
    {
      extern const std::string kMiniPhong;
      extern const std::string kSkybox;
    }
    namespace post_effect_techniques
    {
      extern const std::string kNul;
      extern const std::string kRadialBlur;
      extern const std::string kGreyScale;
      extern const std::string kSepia;
      extern const std::string kInversedColors;
      extern const std::string kCombinedColors;
      extern const std::string kGradualBlur;
      extern const std::string kGrowingPeephole;
      extern const std::string kHorizontalShuttersClose;
      extern const std::string kHorizontalShuttersOpen;
      extern const std::string kRageVision;
      extern const std::string kTiles;
    }
    namespace materials
    {
      extern const std::string kBasic;
    }
    namespace models
    {
      extern const std::string kTurtle;
      extern const std::string kCastle;
      extern const std::string kJellyfish;
      extern const std::string kBucket;
      extern const std::string kRake;
      extern const std::string kShovel;
      extern const std::string kSifter;
      extern const std::string kTree;
      extern const std::string kWall;
    }
  }
}