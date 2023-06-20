#include "Scene.hpp"
#include "Components/ComponentType.hpp"

namespace Game::Scene
{
  char& GetValue(Temp::Scene::Data& data, Temp::Entity entity)
  {
    return Temp::Scene::Get<Game::Component::Type::VALUE>(data, entity);
  }
  
  std::vector<char>& GetCollectedValue(Temp::Scene::Data& data, Temp::Entity entity)
  {
    return Temp::Scene::Get<Game::Component::Type::COLLECTED_VALUE>(data, entity);
  }
  
  namespace Player
  {
    void addScore(Temp::Entity player, Temp::Scene::Data& data, int value)
    {
      Temp::Scene::Get<Game::Component::Type::SCORE>(data, player) += value;
    }
    
    int getScore(Temp::Entity player, Temp::Scene::Data& data)
    {
      return Temp::Scene::Get<Game::Component::Type::SCORE>(data, player);
    }
  }
}

