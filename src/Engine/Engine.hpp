#pragma once

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
    };
    
    void Run(Engine::Data& engine);
    void Destroy(Engine::Data& engine);
  }
}
