#pragma once
#include "Meta.h"
#include "IRenderComponentInterface.h"
#include "GameObjectComponent.h"
namespace PM3D
{
  class HeightMapRenderComponent final :
    public GameObjectComponent, public IRenderComponentInterface
  {
  public:
    HeightMapRenderComponent();
    void Draw() const override;
    bool CanOptimizeLod() const override;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(HeightMapRenderComponent);
  }
}