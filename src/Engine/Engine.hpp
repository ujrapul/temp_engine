#pragma once

#include "Input.hpp"
#include <vector>

namespace Temp
{
  namespace Scene
  {
    struct Data;
  }

  namespace Engine
  {
    struct Data
    {
      std::vector<Scene::Data*> scenes;
      Input::KeyEventData keyEventData;
      bool quit{false};
    };
    
    void Run(Engine::Data& engine, const char* windowName);
    void Destroy(Engine::Data& engine);
    [[nodiscard]] Engine::Data Construct();
    void Quit(Engine::Data& engine);
  }
}
