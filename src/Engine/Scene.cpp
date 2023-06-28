#include "Scene.hpp"

namespace Temp
{
  namespace Scene
  {
    namespace
    {
      void DequeueRender(Scene::Data &scene)
      {
        std::unique_lock<std::mutex> lock(scene.queueMtx);
        while (!scene.renderQueue.empty())
        {
          auto render = scene.renderQueue.front();
          render.func(scene, render.data);
          scene.renderQueue.pop();
        }
      }
    }

    void Construct(Data &scene)
    {
      Coordinator::Init(scene.coordinator);
    }

    void Destruct(Data &scene)
    {
      Coordinator::Destruct(scene.coordinator);
    }

    Entity CreateEntity(Data &scene)
    {
      return Coordinator::CreateEntity(scene.coordinator);
    }

    void DestroyEntity(Data &scene, Entity entity)
    {
      Coordinator::DestroyEntity(scene.coordinator, entity);
    }

    const Math::Vec2f &GetPosition(const Data &scene, Entity entity)
    {
      return Coordinator::GetPosition(scene.coordinator, entity);
    }

    void EnqueueRender(Data &scene, RenderFunction func, void *data)
    {
      std::unique_lock<std::mutex> lock(scene.queueMtx);
      scene.renderQueue.push({func, data});
    }

    void Draw(Data &scene)
    {
      DequeueRender(scene);
      switch (scene.renderState)
      {
      case State::ENTER:
      {
        std::unique_lock<std::mutex> lock(scene.mtx);
        scene.DrawConstructFunc(scene);
        scene.renderState = State::RUN;
        scene.state = Scene::State::RUN;
      }
      break;
      case State::RUN:
      {
        auto &drawableArray = Scene::GetComponentArray<Component::Type::DRAWABLE>(scene);
        for (size_t i = 0; i < drawableArray.mapping.size; ++i)
        {
          Component::Drawable::Draw(drawableArray.array[i]);
        }
        scene.DrawUpdateFunc(scene);
      }
      break;
      case State::LEAVE:
      {
        std::unique_lock<std::mutex> lock(scene.mtx);
        scene.renderState = State::MAX;
        scene.DrawDestructFunc(scene);
        lock.unlock();
        scene.cv.notify_one();
      }
      break;
      default:
        break;
      };
    }

    void ClearRender(Data &scene)
    {
      std::unique_lock<std::mutex> lock(scene.queueMtx);
      scene.renderQueue = {};
    }
  }
}
