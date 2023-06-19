#include "Scene.hpp"

namespace Temp
{
  namespace Scene
  {
    namespace
    {
      void DequeueRender(Scene::Data *scene)
      {
        if (scene->renderQueue.empty())
        {
          return;
        }

        std::scoped_lock<std::mutex> lock(scene->mtx);
        while (!scene->renderQueue.empty())
        {
          auto render = scene->renderQueue.front();
          render.func(render.scene, render.data);
          scene->renderQueue.pop();
        }
      }
    }

    void Construct(Data *data)
    {
      Coordinator::Init(data->coordinator);
    }

    void Destruct(Data *data)
    {
      Coordinator::Destruct(data->coordinator);
    }

    Entity CreateEntity(Data &data)
    {
      return Coordinator::CreateEntity(data.coordinator);
    }

    void DestroyEntity(Data &data, Entity entity)
    {
      Coordinator::DestroyEntity(data.coordinator, entity);
    }

    Math::Vec2f &GetPosition(Data &data, Entity entity)
    {
      return Coordinator::GetPosition(data.coordinator, entity);
    }

    void EnqueueRender(Scene::Data *scene, RenderFunction func, void *data)
    {
      std::scoped_lock<std::mutex> lock(scene->mtx);
      scene->renderQueue.push({func, scene, data});
    }

    void Draw(Data *data)
    {
      DequeueRender(data);
      switch (data->renderState)
      {
      case State::ENTER:
      {
        data->DrawConstructFunc(data);
        data->renderState = State::RUN;
        data->state = Scene::State::RUN;
      }
      break;
      case State::RUN:
      {
        auto *drawableArray = Scene::GetComponentArray<Component::Type::DRAWABLE>(*data);
        for (size_t i = 0; i < drawableArray->mapping.size; ++i)
        {
          Component::Drawable::Draw(&drawableArray->array[i]);
        }
        data->DrawUpdateFunc(data);
      }
      break;
      case State::LEAVE:
      {
        data->renderState = State::MAX;
        data->DrawDestructFunc(data);
      }
      break;
      default:
        break;
      };
    }

    void ClearRender(Data *scene)
    {
      std::scoped_lock<std::mutex> lock(scene->mtx);
      scene->renderQueue = {};
    }
  }
}
