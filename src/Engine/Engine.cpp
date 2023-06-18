#include "Engine.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include <chrono>
#include <thread>
#ifdef __linux__
#include "Render/OpenGL/X11Render.hpp"
#endif

namespace Temp::Engine
{
  void Run(Data &engine, const char *windowName, int windowX, int windowY)
  {
    Scene::Data *currentScene = engine.scenes.front();
    // Start Render Thread
    Render::Initialize(windowName, windowX, windowY, engine);

#ifndef __linux__
    std::thread inputThread(Input::HandleThread, std::ref(engine.keyEventData));
    inputThread.detach();
#endif

    float deltaTime{};
    auto start = std::chrono::high_resolution_clock::now();

    while (currentScene && !engine.quit)
    {
      auto stop = std::chrono::high_resolution_clock::now();
      deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
      start = stop;

      if (!Render::IsInitialized())
      {
        continue;
      }

      // Process events in the renderer
      Render::Run(engine);
      switch (currentScene->state)
      {
      case Scene::State::ENTER:
        Temp::Scene::ClearRender(currentScene);
        if (currentScene->Construct)
          currentScene->Construct(currentScene);
        engine.currentScene = currentScene;
        currentScene->state = Scene::State::RUN;
        break;
      case Scene::State::RUN:
        if (currentScene->Update)
          currentScene->Update(currentScene, deltaTime);
        break;
      case Scene::State::LEAVE:
        Temp::Scene::ClearRender(currentScene);
        if (currentScene->DestructFunc)
          currentScene->DestructFunc(currentScene);
        currentScene->state = Scene::State::ENTER;
        currentScene = currentScene->nextScene;
        engine.currentScene = currentScene;
        if (currentScene)
          currentScene->state = Scene::State::ENTER;
        break;
      default:
        break;
      }

      Input::Process(engine.keyEventData);
    }

    Render::Destroy();
  }

  void Destroy(Data &engine)
  {
    // For now the games should handle clean up of scenes
    for (Scene::Data *scene : engine.scenes)
    {
      delete scene;
    }
    engine.scenes.clear();
  }

  void Construct(Data &engine)
  {
    engine.keyEventData = std::move(Input::Construct());
  }

  void Quit(Data &engine)
  {
    engine.quit = true;
  }

  void EnqueueGlobalRender(Data &engine, RenderFunction func, void *data)
  {
    std::scoped_lock<std::mutex> lock(engine.mtx);
    engine.renderQueue.push({func, data});
  }

  void DequeueGlobalRender(Data &engine)
  {
    if (engine.renderQueue.empty())
    {
      return;
    }

    std::scoped_lock<std::mutex> lock(engine.mtx);
    auto render = engine.renderQueue.front();
    render.func(render.data);
    engine.renderQueue.pop();
  }
}
