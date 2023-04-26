#pragma once

#include "Entity.hpp"
#include "Math.hpp"
#include <array>

namespace Temp
{
  namespace Component
  {
    namespace Transform
    {
      struct Data
      {
        Math::Vec3 position;
        Math::Quat rotation;
        Math::Vec3 scale;
      };

      struct Array
      {
        std::array<Data, MAX_ENTITIES> data;
        // Value = Entity     | Index = Data Index
        std::array<Entity, MAX_ENTITIES> indexToEntity;
        // Value = Data Index | Index = Entity
        std::array<std::size_t, MAX_ENTITIES> entityToIndex;
        std::size_t size;
      };
      
    }
  }
}