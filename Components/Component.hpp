#pragma once

#include <cstdint>
#include <bitset>

namespace Temp
{
  using ComponentType = std::uint8_t;

  const ComponentType MAX_COMPONENTS = 32;

  // Used to track which components an entity has
  using Signature = std::bitset<MAX_COMPONENTS>;

  namespace ComponentManager
  {
    struct Mappings
    {
      std::array<Entity, MAX_ENTITIES> indexToEntity;
      std::array<std::size_t, MAX_ENTITIES> entityToIndex;
    };

    void AddComponent()
    {

    }
  }
}
