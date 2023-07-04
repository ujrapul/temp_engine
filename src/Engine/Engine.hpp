#pragma once

#include "Input.hpp"
#include "Scene.hpp"
#include <vector>
#include <functional>

struct lua_State;

namespace Temp::Coordinator
{
  struct Data;
}

namespace Temp::Engine
{
  struct RenderData
  {
    void (*func)(void*){nullptr};
    void *data;
  };
  
  struct CoordinatorFns
  {
    void (*Init)(Coordinator::Data& data){nullptr};
    void (*Destruct)(Coordinator::Data& data){nullptr};
    void (*Reset)(Coordinator::Data& data){nullptr};
  };
  
  struct Data
  {
    std::vector<Scene::SceneFns> sceneFns{};
    std::queue<RenderData> renderQueue{};
    Input::KeyEventData keyEventData{};
    CoordinatorFns coordinatorFns{};
    Scene::Data *currentScene{nullptr};
    lua_State* lua{nullptr};
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
  void EnqueueGlobalRender(Data& engine, void (*func)(void*), void *data);
  void DequeueGlobalRender(Data& engine);
  bool IsActive(const Data &engine);
}
