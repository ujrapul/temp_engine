#pragma once

#include "Entity.hpp"
#include "Math.hpp"
#include <array>
#include <cassert>

namespace Temp
{
  namespace Component
  {
    struct Mapping
    {
      // Value = Entity     | Index = Data Index
      std::array<Entity, MAX_ENTITIES> indexToEntity;
      // Value = Data Index | Index = Entity
      std::array<std::size_t, MAX_ENTITIES> entityToIndex;
      std::size_t size{};
    };
    
    template<typename T>
    struct ArrayData
    {
      std::array<T, MAX_ENTITIES> array;
      Mapping mapping;
    };
    
    template<typename T>
    void Init(ArrayData<T>& data)
    {
      for (Entity e = 0; e < MAX_ENTITIES; ++e) {
        data.mapping.indexToEntity[e] = __SIZE_MAX__;
        data.mapping.entityToIndex[e] = __SIZE_MAX__;
      }
    }

    template<typename T>
    void Set(
      ArrayData<T>& data,
      Entity entity,
      const T& component)
    {
      assert(entity < MAX_ENTITIES && "Component added to same entity more than once.");

      if (data.mapping.entityToIndex[entity] < MAX_ENTITIES) {
        data.array[entity] = component;
      } else {
        // Put new entry at end and update the maps
        size_t newIndex = data.mapping.size;
        data.mapping.entityToIndex[entity] = newIndex;
        data.mapping.indexToEntity[newIndex] = entity;
        data.array[newIndex] = component;
        ++data.mapping.size;
      }
    }

    template<typename T>
    void Remove(ArrayData<T>& data, Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Removing non-existent component.");

      size_t indexOfRemovedEntity = data.mapping.entityToIndex[entity];
      size_t indexOfLastElement = data.mapping.size - 1;
      data.array[indexOfRemovedEntity] = data.array[indexOfLastElement];
      
      Entity entityOfLastElement = data.mapping.indexToEntity[indexOfLastElement];
      data.mapping.entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
      data.mapping.indexToEntity[indexOfRemovedEntity] = entityOfLastElement;
      
      data.mapping.entityToIndex[entity] = __SIZE_MAX__;
      data.mapping.indexToEntity[indexOfLastElement] = __SIZE_MAX__;
      
      --data.mapping.size;
    }

    template<typename T>
    const T& Get(const ArrayData<T>& data, Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Retrieving non-existent component.");

      return data.array[data.mapping.entityToIndex[entity]];
    }
    
    template<typename T>
    T& Get(ArrayData<T>& data, Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Retrieving non-existent component.");

      return data.array[data.mapping.entityToIndex[entity]];
    }
    
    template<typename T>
    void EntityDestroyed(ArrayData<T>& data, Entity entity)
    {
      if (data.mapping.entityToIndex[entity] < MAX_ENTITIES) {
        Remove<T>(data, entity);
      }
    }
  }
}
