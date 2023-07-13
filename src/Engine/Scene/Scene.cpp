#include "Scene.hpp"
#include "SceneObject.hpp"

namespace Temp::Scene
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
    for (auto &object : scene.objects)
    {
      SceneObject::Construct(scene, object);
    }
    //    Coordinator::Init(scene.coordinator);
  }

  void Destruct(Data &scene)
  {
    for (auto &object : scene.objects)
    {
      SceneObject::Destruct(scene, object);
    }
    //    Coordinator::Destruct(scene.coordinator);
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
      scene.sceneFns.DrawConstructFunc(scene);
      scene.renderState = State::RUN;
      scene.state = Scene::State::RUN;
    }
    break;
    case State::RUN:
    {
      std::lock_guard<std::mutex> lock(scene.mtx);
      auto &drawableArray = Scene::GetComponentArray<Component::Type::DRAWABLE>(scene);
      for (size_t i = 0; i < drawableArray.mapping.size; ++i)
      {
        Component::Drawable::Draw(drawableArray.array[i]);
      }
      scene.sceneFns.DrawUpdateFunc(scene);
#ifdef DEBUG
      if (scene.shadersToReload.size() > 0)
      {
        Render::OpenGLWrapper::LoadShaders();
        for (auto shaderIdx : scene.shadersToReload)
        {
          scene.sceneFns.DrawReloadFunc(scene, shaderIdx);
        }
        scene.shadersToReload.clear();
      }
#endif
    }
    break;
    case State::LEAVE:
    {
      std::unique_lock<std::mutex> lock(scene.mtx);
      scene.renderState = State::MAX;
      scene.sceneFns.DrawDestructFunc(scene);
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
    std::lock_guard<std::mutex> lock(scene.queueMtx);
    scene.renderQueue = {};
  }

  SceneObject::Data &GetObject(Scene::Data &scene, const std::string &name)
  {
    return scene.objects[scene.objectsNameIdxTable[name]];
  }

  void AddObject(Scene::Data &scene, const SceneObject::Data &object)
  {
    std::lock_guard<std::mutex> lock(scene.mtx);
    
    if (!scene.emptyIndexes.empty())
    {
      scene.objects[scene.emptyIndexes.front()] = object;
      scene.objectsNameIdxTable[object.name] = scene.emptyIndexes.front();
      scene.emptyIndexes.pop();
    }
    else
    {
      scene.objects.push_back(object);
      scene.objectsNameIdxTable[object.name] = static_cast<int>(scene.objects.size() - 1);
    }
  }

  void RemoveObject(Scene::Data &scene, const SceneObject::Data &object)
  {
    std::lock_guard<std::mutex> lock(scene.mtx);
    
    size_t i = 0;
    for (auto it = scene.objects.begin(); it != scene.objects.end(); ++it, ++i)
    {
      if (it->name == object.name)
      {
        scene.objects.erase(it);
        scene.emptyIndexes.push(static_cast<int>(i));
        break;
      }
    }
  }
}
