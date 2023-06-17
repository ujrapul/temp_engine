#include "Scene.hpp"

namespace Temp
{
  namespace Scene
  {
    void Destruct(Data *data)
    {
      Temp::Coordinator::Destruct(data->coordinator);
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

    void DequeueRender(Scene::Data *scene)
    {
      if (scene->renderQueue.empty())
      {
        return;
      }

      std::scoped_lock<std::mutex> lock(scene->mtx);
      auto render = scene->renderQueue.front();
      render.func(render.scene, render.data);
      scene->renderQueue.pop();
    }

    void Draw(Data *data)
    {
      DequeueRender(data);
      if (data->state == Scene::State::RUN && data->renderQueue.empty())
      {
        auto *drawableArray = Scene::GetComponentArray<Component::Type::DRAWABLE>(*data);
        for (size_t i = 0; i < drawableArray->mapping.size; ++i)
        {
          Component::Drawable::Draw(&drawableArray->array[drawableArray->mapping.indexToEntity[i]]);
        }
      }
    }
  }
}
