#pragma once

#include "Coordinator.hpp"
#include <iostream>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace Temp::Scene
{
  enum class State : uint8_t
  {
    ENTER = 0,
    RUN = 1,
    LEAVE = 2,
    MAX = 3
  };

  inline void NoOpScene(struct Data &) {}
  inline void NoOpSceneUpdate(struct Data &, float) {}
  void Construct(Data &scene);
  void Destruct(Data &scene);
  void Draw(Data &scene);

  typedef void (*RenderFunction)(Data &, void *);

  struct RenderData
  {
    RenderFunction func;
    void *data;
  };

  struct Data
  {
    Coordinator::Data coordinator{};
    std::queue<RenderData> renderQueue{};
    State state{State::ENTER};
    State renderState{State::MAX};
    Data *nextScene{nullptr};
    void (*Construct)(Scene::Data &){Construct};
    void (*Update)(Scene::Data &, float){NoOpSceneUpdate};
    void (*DestructFunc)(Scene::Data &){Destruct};
    void (*DrawConstructFunc)(Scene::Data &){NoOpScene};
    void (*DrawDestructFunc)(Scene::Data &){NoOpScene};
    void (*DrawUpdateFunc)(Scene::Data &){NoOpScene};
    std::mutex mtx{};
    std::condition_variable cv{};
  };

  template <uint8_t T>
  [[nodiscard]] constexpr Component::MapToComponentDataType<T> &Get(Data &scene, Entity entity)
  {
    return Component::Container::Get<T>(scene.coordinator.componentData, entity);
  }

  template <uint8_t T>
  [[nodiscard]] constexpr const Component::MapToComponentDataType<T> &Get(const Data &scene, Entity entity)
  {
    return Component::Container::Get<T>(scene.coordinator.componentData, entity);
  }

  template <uint8_t T>
  [[nodiscard]] constexpr Entity GetEntityUsingIndex(Data &scene, size_t index)
  {
    return static_cast<Component::ArrayData<Component::MapToComponentDataType<T>> *>(scene.coordinator.componentData.components[T])->mapping.indexToEntity[index];
  }

  template <uint8_t T>
  [[nodiscard]] constexpr size_t GetComponentSize(Data &scene)
  {
    return static_cast<Component::ArrayData<Component::MapToComponentDataType<T>> *>(scene.coordinator.componentData.components[T])->mapping.size;
  }

  template <uint8_t T>
  [[nodiscard]] constexpr Component::ArrayData<Component::MapToComponentDataType<T>> *GetComponentArray(Data &scene)
  {
    return static_cast<Component::ArrayData<Component::MapToComponentDataType<T>> *>(scene.coordinator.componentData.components[T]);
  }

  template <uint8_t T>
  constexpr void AddComponent(Data &scene, Entity entity, Component::MapToComponentDataType<T> component)
  {
    Coordinator::AddComponent<T>(scene.coordinator, entity, component);
  }

  template <uint8_t T>
  constexpr void AddComponent(Data &scene, Entity entity)
  {
    Coordinator::AddComponent<T>(scene.coordinator, entity, Component::GetDefaultValue<T>());
  }

  Entity CreateEntity(Data &scene);
  void DestroyEntity(Data &scene, Entity entity);
  Math::Vec2f &GetPosition(Data &scene, Entity entity);
  void EnqueueRender(Scene::Data &scene, RenderFunction func, void *data);
  void ClearRender(Scene::Data &scene);
}
