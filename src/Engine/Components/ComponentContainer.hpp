#pragma once

#include "ComponentData.hpp"
#include "ComponentType.hpp"

namespace Temp
{
  namespace Component
  {
    namespace Container
    {
      const uint8_t MAX = 128;

      struct Data
      {
        std::array<void*, MAX> components;
      };
      
      template<typename T>
      void Init(Data& data, uint8_t type)
      {
        data.components[type] = new ArrayData<T>();
        Temp::Component::Init
          (*static_cast<Temp::Component::ArrayData<T>*>(data.components[type]));
      }

      void Init(Data& data);
      void Destruct(Data& data);
      void EntityDestroyed(Data& data, Entity entity);
      
      template<typename T>
      T& Get(Data& data, Entity entity, Type type)
      {
        return Component::Get
          (*static_cast<Component::ArrayData<Math::Vec2>* >
            (data.components[static_cast<size_t>(type)]),
          entity);
      }
      
      template<typename T>
      void Set(Data& data, Entity entity, const T& component, uint8_t type)
      {
        Component::Set
          (*static_cast<Component::ArrayData<Math::Vec2>* >(data.components[type]),
           entity,
           component);
      }
    }
  }
}
