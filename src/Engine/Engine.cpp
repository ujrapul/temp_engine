#include "Engine.hpp"
#include "Scene.hpp"
#include <chrono>
#include <thread>
#ifdef __linux__
#include "Render/OpenGL/X11Render.hpp"
#endif

namespace Temp::Engine
{
  void Run(Engine::Data &engine, const char* windowName)
  {
    Scene::Data *currentScene = engine.scenes.front();
    // Start Render Thread
    Render::Initialize(windowName, engine);

#ifndef __linux__
    std::thread inputThread(Input::HandleThread, std::ref(engine.keyEventData));
    inputThread.detach();
#endif

    while (currentScene && !engine.quit)
    {
      // Process events in the renderer
      Render::Run(engine);
      switch (currentScene->state)
      {
      case Scene::State::ENTER:
        if (currentScene->Construct)
          currentScene->Construct(currentScene);
        engine.currentScene = currentScene;
        currentScene->state = Scene::State::RUN;
        break;
      case Scene::State::RUN:
        // if (currentScene->Update)
        //   currentScene->Update(currentScene, 0.01f/*deltaTime*/);
        break;
      case Scene::State::LEAVE:
        if (currentScene->Destruct)
          currentScene->Destruct(currentScene);
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

  void Destroy(Engine::Data &engine)
  {
    // For now the games should handle clean up of scenes
    for (Scene::Data *scene : engine.scenes)
    {
      delete scene;
    }
    engine.scenes.clear();
  }

  Engine::Data Construct()
  {
    Engine::Data out;
    out.keyEventData = std::move(Input::Construct());
    return out;
  }

  void Quit(Engine::Data& engine)
  {
    engine.quit = true;
  }
}
