#pragma once

#include "Coordinator.hpp"
#include <iostream>
#include <functional>
#include <mutex>

namespace Temp
{
  namespace Scene
  {
    enum class State : uint8_t
    {
      ENTER = 0,
      RUN = 1,
      LEAVE = 2,
    };

    struct Data;
    struct RenderData;

    inline void NoOpScene(struct Data *) {}
    inline void NoOpSceneUpdate(struct Data *, float) {}
    void Destruct(Data *data);
    void Draw(Data *data);

    struct Data
    {
      Coordinator::Data coordinator{};
      std::array<Entity, MAX_ENTITIES> entities{};
      std::queue<RenderData> renderQueue{};
      State state{State::ENTER};
      Data *nextScene{nullptr};
      void (*Construct)(Scene::Data *){NoOpScene};
      void (*Update)(Scene::Data *, float){NoOpSceneUpdate};
      void (*DestructFunc)(Scene::Data *){Destruct};
      void (*DrawFunc)(Scene::Data *){Draw};
      std::mutex mtx{};
    };

    typedef std::function<void(Data *, void *)> RenderFunction;

    struct RenderData
    {
      RenderFunction func;
      Scene::Data *scene;
      void *data;
    };

    template <uint8_t T>
    [[nodiscard]] constexpr Component::MapToComponentDataType<T> &Get(Data &data, Entity entity)
    {
      return Component::Container::Get<T>(data.coordinator.componentData, entity);
    }

    template <uint8_t T>
    [[nodiscard]] constexpr Entity GetEntityUsingIndex(Data &data, size_t index)
    {
      return static_cast<Component::ArrayData<Component::MapToComponentDataType<T>> *>(data.coordinator.componentData.components[T])->mapping.indexToEntity[index];
    }

    template <uint8_t T>
    [[nodiscard]] constexpr size_t GetComponentSize(Data &data)
    {
      return static_cast<Component::ArrayData<Component::MapToComponentDataType<T>> *>(data.coordinator.componentData.components[T])->mapping.size;
    }

    template <uint8_t T>
    [[nodiscard]] constexpr Component::ArrayData<Component::MapToComponentDataType<T>> *GetComponentArray(Data &data)
    {
      return static_cast<Component::ArrayData<Component::MapToComponentDataType<T>> *>(data.coordinator.componentData.components[T]);
    }

    template <uint8_t T>
    constexpr void AddComponent(Data &data, Entity entity, Component::MapToComponentDataType<T> component)
    {
      Coordinator::AddComponent<T>(data.coordinator, entity, component);
    }

    template <uint8_t T>
    constexpr void AddComponent(Data &data, Entity entity)
    {
      Coordinator::AddComponent<T>(data.coordinator, entity, Component::GetDefaultValue<T>());
    }

    Entity CreateEntity(Data &data);
    void DestroyEntity(Data &data, Entity entity);
    Math::Vec2f &GetPosition(Data &data, Entity entity);
    void EnqueueRender(Scene::Data *scene, RenderFunction func, void *data);
    void DequeueRender(Scene::Data *scene);
  }
}
