#include "stdafx.h"
#include "LambdaUpdateComponent.h"

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(LambdaUpdateComponent);
    template<>
    component_interface_id_list_t GetComponentInterfaces<LambdaUpdateComponent>() noexcept { return{ GetInterfaceId<IUpdateComponentInterface>() }; }
  }

  LambdaUpdateComponent::LambdaUpdateComponent(std::function<void()> lambda)
  {
    SetLambda(lambda);
  }

  LambdaUpdateComponent::LambdaUpdateComponent() : LambdaUpdateComponent([]() {})
  {
  }

  void LambdaUpdateComponent::SetLambda(std::function<void()> lambda)
  {
    lambda_ = lambda;
  }
  void LambdaUpdateComponent::Update()
  {
    lambda_();
  }
}