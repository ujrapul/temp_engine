#pragma once

#include <vector>

namespace Temp
{
  namespace Scene
  {
    struct Data;
  }
  
  namespace Input
  {
    struct KeyEventData;
  }

  namespace Engine
  {
    struct Data
    {
      std::vector<Scene::Data*> scenes;
      Input::KeyEventData* keyEventData;
    };
    
    void Run(Engine::Data& engine);
    void Destroy(Engine::Data& engine);
  }
}
