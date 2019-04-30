#pragma once
#include "Meta.h"

#include <memory>
#include <map>

namespace PM3D
{
  template<typename T>
  class ComponentObject
  {
  public:
    template<typename U, class... Args>
    U* AddComponent(Args&&... args)
    {
      std::unique_ptr<U> s = std::make_unique<U>(std::forward<Args>(args)...);
      s->OnAttached(*this);
      meta::component_id_t comp_id = meta::GetComponentId<U>();
      from_component_id_to_component_[ comp_id ] = std::move(s);
      for ( meta::component_interface_id_t i : meta::GetComponentInterfaces<U>() )
      {
        if ( from_interface_id_to_component_id_.find(i) != from_interface_id_to_component_id_.end() )
        {
          throw std::out_of_range{ "This already has a component which supports this interface!" };
        }
        from_interface_id_to_component_id_[ i ] = comp_id;
      }
      return dynamic_cast<U*>( from_component_id_to_component_[ comp_id ].get() );
    }
    template<typename U>
    void RemoveComponent()
    {
      for ( auto& i : meta::GetComponentInterfaces<U>() )
      {
        from_interface_id_to_component_id_.erase(i);
      }
      from_component_id_to_component_.erase(meta::GetComponentId<U>());
    }
    template<typename I>
    I* As()
    {
      meta::component_interface_id_t interface_id = meta::GetInterfaceId<I>();
      if ( from_interface_id_to_component_id_.find(interface_id) != from_interface_id_to_component_id_.end() )
      {
        meta::component_id_t comp_id = from_interface_id_to_component_id_[ interface_id ];
        return dynamic_cast<I*>( from_component_id_to_component_[ comp_id ].get() );
      }
      else
      {
        return nullptr;
      }
    }
    template<typename I>
    const I* As() const
    {
      return As<I>();
    }
    template<typename C>
    C* AsComponent()
    {
      meta::component_id_t cmp_id = meta::GetComponentId<C>();
      if ( from_component_id_to_component_.find(cmp_id) != from_component_id_to_component_.end() )
      {
        return dynamic_cast<C*>( from_component_id_to_component_[ cmp_id ].get() );
      }
      else
      {
        return nullptr;
      }
    }
    template<typename C>
    const C* AsComponent() const
    {
      return AsComponent<C>();
    }
    virtual ~ComponentObject()
    {
      for ( auto& s : from_component_id_to_component_ )
      {
        s.second->OnDetached(*this);
      }
    }
  private:
    std::map<meta::component_interface_id_t, meta::component_id_t> from_interface_id_to_component_id_;
    std::map<meta::component_id_t, std::unique_ptr<T>> from_component_id_to_component_;
  };
}