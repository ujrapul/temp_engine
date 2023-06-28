#pragma once

#include "Input.hpp"
#include <vector>
#include <functional>

namespace Temp
{
  namespace Scene
  {
    struct Data;
  }

  namespace Engine
  {
    typedef std::function<void(void *)> RenderFunction;

    struct RenderData
    {
      RenderFunction func;
      void *data;
    };

    struct Data
    {
      std::vector<Scene::Data *> scenes{};
      std::queue<RenderData> renderQueue{};
      Input::KeyEventData keyEventData{};
      Scene::Data *currentScene{nullptr};
      std::mutex mtx{};
      bool quit{false};
    };

    inline Temp::Engine::Data engine{};

    void Start(Data &engine, const char *windowName, int windowX, int windowY);
    void Process(Data &engine);
    void Run(Data &engine, const char *windowName, int windowX, int windowY);
    void Destroy(Data &engine);
    void Construct(Data& engine);
    void Quit(Data &engine);
    void EnqueueGlobalRender(Data& engine, RenderFunction func, void *data);
    void DequeueGlobalRender(Data& engine);
    bool IsActive(const Data &engine);
  }
}
