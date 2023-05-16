#pragma once

#include "Coordinator.hpp"
#include <iostream>

namespace Temp
{
  namespace Scene
  {
    enum class State : uint8_t
    {
      ENTER = 0,
      RUN   = 1,
      LEAVE = 2,
    };
    
    struct Data
    {
      Coordinator::Data coordinator{};
      std::array<Entity, MAX_ENTITIES> entities{};
      State state{State::ENTER};
      Data* nextScene{nullptr};
      void (*Construct)(Scene::Data*){nullptr};
      void (*Update)(Scene::Data*, float){nullptr};
      void (*Destruct)(Scene::Data*){nullptr};
    };
    
    template<uint8_t T>
    Component::MapToComponentDataType<T>& Get(Data& data, Entity entity)
    {
      return Component::Container::Get<T>(data.coordinator.componentData, entity);
    }
    
    template<uint8_t T>
    Entity GetEntityUsingIndex(Data& data, size_t index)
    {
      return static_cast<Component::ArrayData<Component::MapToComponentDataType<T>>*>
        (data.coordinator.componentData.components[T])->mapping.indexToEntity[index];
    }
    
    template<uint8_t T>
    size_t GetComponentSize(Data& data)
    {
      return static_cast<Component::ArrayData<Component::MapToComponentDataType<T>>*>
        (data.coordinator.componentData.components[T])->mapping.size;
    }
    
    template<uint8_t T>
    void AddComponent(Data& data, Entity entity, Component::MapToComponentDataType<T> component)
    {
      Coordinator::AddComponent<T>(data.coordinator, entity, component);
    }
    
    void Destruct(Data& data);
    Entity CreateEntity(Data& data);
    void DestroyEntity(Data& data, Entity entity);
    Math::Vec2& GetPosition(Data& data, Entity entity);
  }
}
