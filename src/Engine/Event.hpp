#pragma once

#include "Engine.hpp"
#include "Hoverable.hpp"
#include "FontLoader.hpp"
#include "Logger.hpp"
#include "Scene.hpp"
#include <thread>

namespace Temp::Event
{
  struct Data
  {
    std::thread renderThread{};
    clock_t renderBegin{clock()};
    const float fps60{0.0166666 * 2};
    const float fps30{0.0333333 * 2};
    int windowWidth{};
    int windowHeight{};
    bool renderQuit{false};
    bool renderInitialized{false};
    bool limitFps{false};
    bool isInFullScreen{false};
  };
  
  inline Data EventData{};
  
  inline void Hover(float mouseX, float mouseY)
  {
    auto *scene = Temp::Engine::engine.currentScene;
    if (!scene)
    {
      return;
    }
    
    std::lock_guard<std::mutex> sceneLock(scene->mtx);
    if (scene->state == Scene::State::RUN)
    {
      auto &hoverableArray = Scene::GetComponentArray<Component::Type::HOVERABLE>(*scene);
      for (size_t i = 0; i < hoverableArray.mapping.size; ++i)
      {
        auto &hoverable = hoverableArray.array[i];
        if (Component::Hoverable::IsInside(hoverable, mouseX, mouseY))
        {
          Component::Hoverable::HoverableEnter(*scene, hoverable);
        }
        else
        {
          Component::Hoverable::HoverableLeave(*scene, hoverable);
        }
      }
    }
  }
  
  inline void Click(float mouseX, float mouseY)
  {
    auto *scene = Temp::Engine::engine.currentScene;
    if (!scene)
    {
      return;
    }
    
    std::lock_guard<std::mutex> sceneLock(scene->mtx);
    if (scene->state == Scene::State::RUN)
    {
      auto &hoverableArray = Scene::GetComponentArray<Component::Type::HOVERABLE>(*scene);
      for (size_t i = 0; i < hoverableArray.mapping.size; ++i)
      {
        auto &hoverable = hoverableArray.array[i];
        if (Component::Hoverable::IsInside(hoverable, mouseX, mouseY))
        {
          hoverable.Click(*scene, hoverable);
        }
      }
    }
  }
  
  inline void Resize(void *)
  {
#ifdef __linux__
    glViewport(0, 0, EventData.windowWidth, EventData.windowHeight);
#endif
  }
  
  inline void RenderSetup()
  {
    Logger::Log(glGetString(GL_VERSION));
    
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Temp::Font::LoadFont();
    Temp::Render::OpenGLWrapper::LoadShaders();
    
    EventData.renderInitialized = true;
    
    Resize(nullptr);
  
    EventData.renderBegin = clock();
  }
  
  inline void RenderRun()
  {
    auto &engine = Temp::Engine::engine;

    clock_t end{clock()};
    if (EventData.limitFps && ((float)(end - EventData.renderBegin) / CLOCKS_PER_SEC) < EventData.fps60)
    {
      return;
    }
    EventData.renderBegin = clock();
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Avoid using the Render Queue for real-time updates to avoid flickering!
    Temp::Engine::DequeueGlobalRender(engine);
    if (engine.currentScene) [[likely]]
    {
      Temp::Scene::Draw(*engine.currentScene);
    }
  }
  
  inline void RenderClean()
  {
    // TODO: Clean up resources
  }
}
