#pragma once
#include "Meta.h"
#include "GameObjectComponent.h"
#include "IUpdateComponentInterface.h"

#include <functional>
namespace PM3D
{
  class LambdaUpdateComponent final :
    public GameObjectComponent, public IUpdateComponentInterface
  {
  public:
    LambdaUpdateComponent();
    LambdaUpdateComponent(std::function<void()> lambda);
    void SetLambda(std::function<void()> lambda);
    void Update() override;
  private:
    std::function<void()> lambda_;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(LambdaUpdateComponent);
  }
}
