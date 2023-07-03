#include "Engine.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Event.hpp"
#include "Luable.hpp"
#include <chrono>
#include <thread>
#include <filesystem>
#ifdef __linux__
#include "Render/OpenGL/X11Render.hpp"
#elif __APPLE__
#include "Render/OpenGL/NSRender.hpp"
#endif

namespace Temp::Engine
{
  namespace
  {
    // Exists only because I'm lazy
    Scene::Data *currentScene{nullptr};
    std::thread inputThread{};
#ifdef DEBUG
    std::thread luaThread{};
    std::vector<Component::Luable::Data*> hotReloadLuables;
#endif
    
    void RunLinux(Data &engine, const char *windowName, int windowX, int windowY)
    {
      Start(engine, windowName, windowX, windowY);

      while (currentScene && !engine.quit)
      {
        Process(engine);
      }

      Destroy(engine);
    }
    
#ifdef DEBUG
    void LuaThread(Data& engine)
    {
      while (currentScene && !engine.quit)
      {
        {
          std::lock_guard<std::mutex> sceneLock(currentScene->mtx);
          if (currentScene->state == Scene::State::RUN)
          {
            auto &luableArray = Scene::GetComponentArray<Component::Type::LUABLE>(*currentScene);
            for (size_t i = 0; i < luableArray.mapping.size; ++i)
            {
              auto &luable = luableArray.array[i];
              auto time = std::filesystem::last_write_time(luable.path);
              if (time != luable.time)
              {
                luable.time = time;
                hotReloadLuables.push_back(&luable);
              }
            }
          }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    }
  }
#endif
  
  void Start(Data &engine, const char *windowName, int windowX, int windowY)
  {
    currentScene = engine.scenes.front();
    // Start Render Thread
    Render::Initialize(windowName, windowX, windowY, engine);

#ifndef __linux__
    inputThread = std::thread(Input::HandleThread, std::ref(engine.keyEventData));
#endif
    
    engine.lua = luaL_newstate();
    luaL_openlibs(engine.lua);
    
#ifdef DEBUG
    luaThread = std::thread(LuaThread, std::ref(engine));
#endif
  }
  
  void Process(Data &engine)
  {
    static float deltaTime{};
    static auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
    start = stop;

    if (!Event::EventData.renderInitialized)
    {
      return;
    }

    switch (currentScene->state)
    {
    case Scene::State::ENTER:
    {
      Temp::Scene::ClearRender(*currentScene);
      std::unique_lock<std::mutex> lock(currentScene->mtx);
      currentScene->ConstructFunc(*currentScene);
      engine.currentScene = currentScene;
      currentScene->state = Scene::State::MAX;
      currentScene->renderState = Scene::State::ENTER;
    }
    break;
    case Scene::State::RUN:
      currentScene->Update(*currentScene, deltaTime);
      break;
    case Scene::State::LEAVE:
    {
      Temp::Scene::ClearRender(*currentScene);
      std::unique_lock<std::mutex> lock(currentScene->mtx);
      currentScene->renderState = Scene::State::LEAVE;
      currentScene->cv.wait(lock, []()
                            { return currentScene->renderState == Scene::State::MAX; });
      currentScene->DestructFunc(*currentScene);
      currentScene->state = Scene::State::ENTER;
      currentScene = currentScene->nextScene;
      engine.currentScene = currentScene;
      if (currentScene)
        currentScene->state = Scene::State::ENTER;
    }
    break;
    default:
      break;
    }
    
#ifdef DEBUG
    // Hot Reload Lua Scripts
    {
      std::lock_guard<std::mutex> lock(engine.mtx);
      if (hotReloadLuables.size() > 0)
      {
        for (const auto* luable : hotReloadLuables)
        {
          Component::Luable::LoadScript(*luable);
        }
        hotReloadLuables.clear();
      }
    }
#endif

    // Process events in the renderer
    Render::Run(engine);
    Input::Process(engine.keyEventData);
  }
  
  void Run(Data &engine, const char *windowName, int windowX, int windowY)
  {
#ifdef __linux__
    RunLinux(engine, windowName, windowX, windowY);
#elif __APPLE__
    Render::Run(engine, windowName, windowX, windowY);
#endif
  }

  void Destroy(Data &engine)
  {
#ifndef __linux__
    // TODO: Add a mutex here
    Input::EndInput();
    inputThread.join();
#endif
#ifdef DEBUG
    luaThread.join();
#endif
    Render::Destroy();
    // For now the games should handle clean up of scenes
    for (Scene::Data *scene : engine.scenes)
    {
      delete scene;
    }
    engine.scenes.clear();
    Input::Destruct(engine.keyEventData);
    lua_close(engine.lua);
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
    std::unique_lock<std::mutex> lock(engine.mtx);
    engine.renderQueue.push({func, data});
  }

  void DequeueGlobalRender(Data &engine)
  {
    if (engine.renderQueue.empty())
    {
      return;
    }

    std::unique_lock<std::mutex> lock(engine.mtx);
    auto render = engine.renderQueue.front();
    render.func(render.data);
    engine.renderQueue.pop();
  }
  
  bool IsActive(const Data &engine)
  {
    return !engine.quit && currentScene;
  }
}
