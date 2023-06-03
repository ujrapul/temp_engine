#include "Engine.hpp"
#include "Scene.hpp"
#include <chrono>
#include <thread>
#ifdef __linux__
#include "Render/glad/glx.h"
#include "Render/OpenGL/X11Render.hpp"
#endif

namespace Temp::Engine
{
  void Run(Engine::Data &engine, const char* windowName)
  {
    Render::Initialize(windowName);
    Scene::Data *currentScene = engine.scenes.front();

#ifndef __linux__
    std::thread inputThread(Input::HandleThread, std::ref(engine.keyEventData));
    inputThread.detach();
#endif

    while (currentScene && !engine.quit)
    {
      Render::Run(engine);
      switch (currentScene->state)
      {
      case Scene::State::ENTER:
        if (currentScene->Construct)
          currentScene->Construct(currentScene);
        currentScene->state = Scene::State::RUN;
        break;
      case Scene::State::RUN:
        // if (currentScene->Update)
        //   currentScene->Update(currentScene, deltaTime);
        break;
      case Scene::State::LEAVE:
        if (currentScene->Destruct)
          currentScene->Destruct(currentScene);
        currentScene->state = Scene::State::ENTER;
        currentScene = currentScene->nextScene;
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
