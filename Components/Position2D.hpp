#pragma once

#include "Entity.hpp"
#include "Math.hpp"
#include <array>
#include <cassert>

namespace Temp
{
  namespace Component
  {
    struct Position2D
    {
      std::array<float, MAX_ENTITIES> x;
      std::array<float, MAX_ENTITIES> y;
      std::size_t size;
    };

    // void Insert(
    //   Data& data,
    //   Entity entity,
    //   Math::Vec2 component)
    // {
    //   assert(entity < MAX_ENTITIES && "Component added to same entity more than once.");

    //   data.x[entity] = component.x;
    //   data.y[entity] = component.y;
    //   data.indexToEntity[entity] = entity;
    //   data.entityToIndex[entity] = entity;
    //   ++data.size;
    // }

    // void Remove(
    //   Data& data,
    //   Entity entity)
    // {
    //   assert(entity < MAX_ENTITIES && "Removing non-existent component.");

    //   std::size_t indexOfRemovedEntity = data.entityToIndex[entity];
    //   std::size_t indexOfLastElement = data.size - 1;
    //   data.x[indexOfRemovedEntity] = data.x[indexOfLastElement];
    //   data.y[indexOfRemovedEntity] = data.y[indexOfLastElement];

    //   Entity entityOfLastElement = data.indexToEntity[indexOfLastElement];
    //   data.entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
    //   data.indexToEntity[indexOfRemovedEntity] = entityOfLastElement;
    //   --data.size;
    // }

    // Math::Vec2 Get(const Data& data, Entity entity)
    // {
    //   assert(entity < MAX_ENTITIES && "Retrieving non-existent component.");

    //   return {data.x[data.entityToIndex[entity]], data.y[data.entityToIndex[entity]]};
    // }

    // void EntityDestroyed(Data& data, Entity entity)
    // {
    //   Remove(data, entity);
    // }
  }
}