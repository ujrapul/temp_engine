#pragma once

#include "Engine/Scene.hpp"

namespace Game::Scene
{
  char& GetValue(Temp::Scene::Data& data, Temp::Entity entity);
  std::vector<char>& GetCollectedValue(Temp::Scene::Data& data, Temp::Entity entity);
  
  namespace Player
  {
    struct Data
    {
      Temp::Entity entity{};
    };
    
    void addScore(Temp::Entity player, Temp::Scene::Data& data, int value);
    int getScore(Temp::Entity player, Temp::Scene::Data& data);
  }
}
