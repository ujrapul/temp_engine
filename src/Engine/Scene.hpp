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

    struct RenderConstructData;
    struct RenderUpdateData;

    inline void NoOpConstruct(struct Data *) {}
    inline void NoOpUpdate(struct Data *, float) {}

    struct Data
    {
      Coordinator::Data coordinator{};
      std::array<Entity, MAX_ENTITIES> entities{};
      std::queue<RenderConstructData> renderConstructQueue{};
      State state{State::ENTER};
      Data *nextScene{nullptr};
      void (*Construct)(Scene::Data *){NoOpConstruct};
      void (*Update)(Scene::Data *, float){NoOpUpdate};
      void (*Destruct)(Scene::Data *){Destruct};
      void (*Draw)(Scene::Data *){Draw};
      std::mutex mtx{};
    };

    typedef std::function<void(Data *, void *)> RenderConstructFunction;

    struct RenderConstructData
    {
      RenderConstructFunction func;
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

    void Destruct(Data &data);
    Entity CreateEntity(Data &data);
    void DestroyEntity(Data &data, Entity entity);
    Math::Vec2f &GetPosition(Data &data, Entity entity);

    inline void EnqueueRenderConstruct(Scene::Data *scene, RenderConstructFunction func, void *data)
    {
      std::scoped_lock<std::mutex> lock(scene->mtx);
      scene->renderConstructQueue.push({func, scene, data});
    }

    inline void DequeueRenderConstruct(Scene::Data *scene)
    {
      if (scene->renderConstructQueue.empty())
      {
        return;
      }

      std::scoped_lock<std::mutex> lock(scene->mtx);
      auto construct = scene->renderConstructQueue.front();
      construct.func(construct.scene, construct.data);
      scene->renderConstructQueue.pop();
    }

    inline void Draw(Data *data)
    {
      DequeueRenderConstruct(data);
      if (data->state == Scene::State::RUN)
      {
        static auto *drawableArray = Scene::GetComponentArray<Component::Type::DRAWABLE>(*data);
        for (size_t i = 0; i < drawableArray->mapping.size; ++i)
        {
          Component::Drawable::Draw(&drawableArray->array[drawableArray->mapping.indexToEntity[i]]);
        }
      }
    }
  }
}
