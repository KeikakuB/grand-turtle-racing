#pragma once
#include <memory>
#include <vector>
#include <string>

namespace PM3D
{
  namespace meta
  {
    // References

#define ADD_REF_TYPE_USING(class_name) using ref_type = class_name*

#define ADD_WEAK_REF_TYPE_USING(class_name) using weak_ref_type = class_name*

    // Components
    using component_id_t = std::string;
    using component_interface_id_t = std::string;
    using component_interface_id_list_t = std::vector<component_interface_id_t>;

    template<class T>
    component_interface_id_t GetInterfaceId() noexcept;
    template<class T>
    component_id_t GetComponentId() noexcept;
    template<class T>
    component_interface_id_list_t GetComponentInterfaces() noexcept;

#define REGISTER_INTERFACE_H_ID(interface_name) template<> component_interface_id_t GetInterfaceId<interface_name>() noexcept;

#define REGISTER_INTERFACE_CPP_ID(interface_name) template<> component_interface_id_t GetInterfaceId<interface_name>() noexcept { return #interface_name; }

#define REGISTER_COMPONENT_H_ID(component_name) \
    template<> component_id_t GetComponentId<component_name>() noexcept; \
    template<> component_interface_id_list_t GetComponentInterfaces<component_name>() noexcept;

#define REGISTER_COMPONENT_CPP_ID(component_name) \
    template<> component_id_t GetComponentId<component_name>() noexcept { return #component_name; }
  }
}