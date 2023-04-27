#include "Components/ComponentContainer.hpp"
#include "EntityManager.hpp"
#include <iostream>

namespace Temp
{
  namespace Engine
  {
    void Update(Temp::Component::Container::Data* componentData,
                Temp::EntityManager::Data* entityData)
    {
      std::cout << "Position: "
        << Temp::Component::Container::getPosition2D(MAX_ENTITIES/2, componentData).x
        << " "
        << Temp::Component::Container::getPosition2D(MAX_ENTITIES/2, componentData).y
        << " "
        << rand() % 999999
        << std::endl;
    }
  
    void Run()
    {
      Temp::Component::Container::Data componentData;
      Temp::EntityManager::Data entityData;
      
      while (true)
      {
        Update(&componentData, &entityData);
      }
    }
  }
}
