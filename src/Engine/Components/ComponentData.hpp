// Built with reference from Austin Morlan's ECS
// https://austinmorlan.com/posts/entity_component_system/
// https://code.austinmorlan.com/austin/2019-ecs

#pragma once

#include "Entity.hpp"
#include "Math.hpp"
#include "Logger.hpp"
#include <array>
#include <cassert>
#include <climits>
#include <mutex>

namespace Temp
{
  namespace Component
  {
    template<typename T>
    inline T dummy{};
    
    struct Mapping
    {
      // Value = Entity     | Index = Data Index
      std::array<Entity, MAX_ENTITIES> indexToEntity{};
      // Value = Data Index | Index = Entity
      std::array<std::size_t, MAX_ENTITIES> entityToIndex{};
      std::size_t size{};
    };
    
    template<typename T>
    struct ArrayData
    {
      // This is using Indexes not Entities!
      std::array<T, MAX_ENTITIES> array{};
      Mapping mapping{};
      std::mutex mtx{};
    };
    
    template<typename T>
    inline void Reset(ArrayData<T>& data)
    {
      // NOTE: Shouldn't be necessary
      // data.array.fill({});
      Init(data);
    }
    
    template<typename T>
    inline void Init(ArrayData<T>& data)
    {
      std::lock_guard<std::mutex> lock(data.mtx);
      data.mapping.indexToEntity.fill({UINT_MAX});
      data.mapping.entityToIndex.fill({SIZE_MAX});
      data.mapping.size = 0;
    }

    template<typename T>
    inline void Set(
      ArrayData<T>& data,
      Entity entity,
      const T& component)
    {
      assert(entity < MAX_ENTITIES && "Component added to same entity more than once.");

      std::lock_guard<std::mutex> lock(data.mtx);
      if (data.mapping.entityToIndex[entity] < MAX_ENTITIES) {
        data.array[data.mapping.entityToIndex[entity]] = component;
      } else {
        // Put new entry at end and update the maps
        std::size_t newIndex = data.mapping.size;
        data.mapping.entityToIndex[entity] = newIndex;
        data.mapping.indexToEntity[newIndex] = entity;
        data.array[newIndex] = component;
        ++data.mapping.size;
      }
    }

    template<typename T>
    inline void Remove(ArrayData<T>& data, Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Removing non-existent component.");

      std::lock_guard<std::mutex> lock(data.mtx);

      std::size_t indexOfRemovedEntity = data.mapping.entityToIndex[entity];
      std::size_t indexOfLastElement = data.mapping.size - 1;
      data.array[indexOfRemovedEntity] = data.array[indexOfLastElement];
      
      Entity entityOfLastElement = data.mapping.indexToEntity[indexOfLastElement];
      data.mapping.entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
      data.mapping.indexToEntity[indexOfRemovedEntity] = entityOfLastElement;
      
      data.mapping.entityToIndex[entity] = SIZE_MAX;
      data.mapping.indexToEntity[indexOfLastElement] = UINT_MAX;
      
      --data.mapping.size;
      
      data.array[data.mapping.size] = {};
    }

    template<typename T>
    [[nodiscard]] constexpr const T& Get(const ArrayData<T>& data, Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Retrieving non-existent component.");

      if (data.mapping.entityToIndex[entity] < MAX_ENTITIES)
      {
        return data.array[data.mapping.entityToIndex[entity]];
      }
      Logger::Log("Warning! Accessing invalid entity!");
      return dummy<T>;
    }
    
    template<typename T>
    [[nodiscard]] constexpr T& Get(ArrayData<T>& data, Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Retrieving non-existent component.");

      if (data.mapping.entityToIndex[entity] < MAX_ENTITIES)
      {
        return data.array[data.mapping.entityToIndex[entity]];
      }
      Logger::Log("Warning! Accessing invalid entity!");
      return dummy<T>;
    }
    
    template<typename T>
    inline void EntityDestroyed(ArrayData<T>& data, Entity entity)
    {
      if (data.mapping.entityToIndex[entity] < MAX_ENTITIES) {
        Remove<T>(data, entity);
      }
    }
  }
}
