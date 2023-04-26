#pragma once

#include "Entity.hpp"
#include "Math.hpp"
#include <array>

namespace Temp
{
  namespace Component
  {
    namespace DataManipulator
    {
      template<typename T>
      void Insert(
        std::array<T, MAX_ENTITIES> data,
        std::size_t size,
        Entity entity,
        const T& component)
      {
        assert(entity < MAX_ENTITIES && "Component added to same entity more than once.")

        data[entity] = component;
      }

      void Remove(Entity entity)
      {

      }

      template<typename T>
      T& Get(Entity entity)
      {

      }

      void EntityDestroyed(Entity entity)
      {
        
      }
    }
  }
}