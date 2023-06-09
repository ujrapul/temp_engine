#include "Engine.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Event.hpp"
#include "Luable.hpp"
#include "SceneObject.hpp"
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
    std::thread hotReloadThread{};
    std::vector<Component::Luable::Data *> hotReloadLuables;
    std::mutex hotReloadLuableMtx{};
#endif

#ifdef __linux
    void RunLinux(Data &engine, const char *windowName, int windowX, int windowY)
    {
      Start(engine, windowName, windowX, windowY);

      while (!Event::RenderInitialized())
      {
        continue;
      }

      while (currentScene)
      {
        {
          {
            std::lock_guard<std::mutex> lock(engine.mtx);
            if (engine.quit)
            {
              break;
            }
          }
          Process(engine);
        }
      }

      Destroy(engine);
    }
#endif

#ifdef DEBUG
    void HotReloadThread(Data &engine)
    {
      const auto &globals = Render::OpenGLWrapper::GlobalShaderFiles();
      auto &globalShaderFilesTimes = Render::OpenGLWrapper::GlobalShaderFilesTimes();

      while (currentScene)
      {
        {
          std::lock_guard<std::mutex> lock(engine.mtx);
          if (engine.quit)
          {
            return;
          }
        }

        {
          std::lock_guard<std::mutex> sceneLock(currentScene->mtx);
          if (currentScene->state == Scene::State::RUN)
          {
            {
              std::lock_guard<std::mutex> hotReloadLuableLock(hotReloadLuableMtx);
              auto &luableArray = Scene::GetComponentArray<Component::Type::LUABLE>(*currentScene);
              for (size_t i = 0; i < luableArray.mapping.size; ++i)
              {
                auto &luable = luableArray.array[i];
                auto time = std::filesystem::last_write_time(AssetsDirectory() / "LuaScripts" / luable.path);
                if (time != luable.time)
                {
                  luable.time = time;
                  hotReloadLuables.push_back(&luable);
                }
              }
            }

            for (size_t i = 0; i < globals.size(); ++i)
            {
              auto &drawableArray = Scene::GetComponentArray<Component::Type::DRAWABLE>(*currentScene);
              auto time = std::filesystem::last_write_time(globals[i].c_str());
              if (time != globalShaderFilesTimes[i])
              {
                globalShaderFilesTimes[i] = time;
                for (size_t i = 0; i < drawableArray.mapping.size; ++i)
                {
                  auto &drawable = drawableArray.array[i];
                  currentScene->shadersToReload.insert(drawable.shaderIdx);
                }
              }
              else
              {
                for (size_t i = 0; i < drawableArray.mapping.size; ++i)
                {
                  auto &drawable = drawableArray.array[i];
                  auto time = std::filesystem::last_write_time(drawable.shaderPath);
                  if (time != drawable.time)
                  {
                    drawable.time = time;
                    currentScene->shadersToReload.insert(drawable.shaderIdx);
                  }
                }
              }
            }
          }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    }
#endif
  }

  void Start(Data &engine, const char *windowName, int windowX, int windowY)
  {
    SceneObject::Init();
    currentScene = new Scene::Data();
    engine.currentScene = currentScene;
    currentScene->sceneFns = engine.sceneFns.front();
    //    Scene::Construct(*currentScene);
    engine.coordinatorFns.Init(currentScene->coordinator);
    // Start Render Thread
    Render::Initialize(windowName, windowX, windowY, engine);

    std::lock_guard<std::mutex> lock(engine.mtx);
#ifndef __linux__
    inputThread = std::thread(Input::HandleThread, std::ref(engine.keyEventData));
#endif

    engine.lua = luaL_newstate();
    luaL_openlibs(engine.lua);

#ifdef DEBUG
    hotReloadThread = std::thread(HotReloadThread, std::ref(engine));
#endif
  }

  void Process(Data &engine)
  {
    static float deltaTime{};
    static auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
    start = stop;

    Scene::State sceneState;
    {
      std::lock_guard<std::mutex> lock(currentScene->mtx);
      sceneState = currentScene->state;
    }
    switch (sceneState)
    {
    case Scene::State::ENTER:
    {
      Temp::Scene::ClearRender(*currentScene);
      // This should always run before DrawConstruct
      currentScene->sceneFns.ConstructFunc(*currentScene);
      
      std::lock_guard<std::mutex> lock(currentScene->mtx);
      currentScene->state = Scene::State::MAX;
      currentScene->renderState = Scene::State::ENTER;
    }
    break;
    case Scene::State::RUN:
      currentScene->sceneFns.Update(*currentScene, deltaTime);
      break;
    case Scene::State::LEAVE:
    {
      Temp::Scene::ClearRender(*currentScene);
      std::unique_lock<std::mutex> lock(currentScene->mtx);
      currentScene->renderState = Scene::State::LEAVE;
      currentScene->cv.wait(lock, []()
                            { return currentScene->renderState == Scene::State::MAX; });
      currentScene->sceneFns.DestructFunc(*currentScene);
      currentScene->sceneFns = *currentScene->sceneFns.nextScene;
      {
        std::lock_guard<std::mutex> lock(engine.mtx);
        engine.coordinatorFns.Reset(currentScene->coordinator);
      }
      currentScene->state = Scene::State::ENTER;
    }
    break;
    default:
      break;
    }

#ifdef DEBUG
    // Hot Reload Lua Scripts
    {
      std::lock_guard<std::mutex> hotReloadLuableLock(hotReloadLuableMtx);
      if (hotReloadLuables.size() > 0)
      {
        for (const auto *luable : hotReloadLuables)
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
    hotReloadThread.join();
#endif
    Render::Destroy();
    // For now the games should handle clean up of scenes
    Render::OpenGLWrapper::ClearShaderStrings();
    Input::Destruct(engine.keyEventData);
    lua_close(engine.lua);
    engine.coordinatorFns.Destruct(currentScene->coordinator);
    delete currentScene;
  }

  void Construct(Data &engine)
  {
    engine.keyEventData = Input::Construct();
  }

  void Quit(Data &engine)
  {
    std::lock_guard<std::mutex> lock(engine.mtx);
    engine.quit = true;
  }

  void EnqueueGlobalRender(Data &engine, void (*func)(void *), void *data)
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
