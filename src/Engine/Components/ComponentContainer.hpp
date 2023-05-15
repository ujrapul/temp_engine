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
      
      template<uint8_t T, typename Out>
      void Init(Data& data)
      {
        data.components[T] = new ArrayData<Out>();
        Temp::Component::Init
          (*static_cast<Temp::Component::ArrayData<Out>*>(data.components[T]));
      }
      
      template<uint8_t T, typename Out>
      ArrayData<Out>* GetComponentArray(Data& data)
      {
        return static_cast<Temp::Component::ArrayData<Out>*>(data.components[T]);
      }

      void Init(Data& data);
      void Destruct(Data& data);
      void EntityDestroyed(Data& data, Entity entity);
      
      template<uint8_t T, typename Out>
      Out& Get(Data& data, Entity entity)
      {
        // This mess is only needed to make sure we don't need to reference the type twice
        // in the function (once for enum and another for assigned type)
        return Component::Get
          (*static_cast<Component::ArrayData<Out>* >(data.components[T]),
          entity);
      }
      
      template<uint8_t T, typename Out>
      void Set(Data& data, Entity entity, const Out& component)
      {
        Component::Set
          (*static_cast<Component::ArrayData<Out>* >(data.components[T]),
           entity,
           component);
      }
    }
  }
}
