#include "Components/ComponentContainer.hpp"
#include "Components/ComponentType.hpp"
#include "EntityManager.hpp"
#include <iostream>
#include <chrono>

namespace Temp
{
  namespace Engine
  {
    void Update(Component::Container::Data& componentData,
                EntityManager::Data& entityData,
                std::array<Entity, MAX_ENTITIES>& entities,
                float deltaTime)
    {
      static float currTime = 0;
      static Entity entity = 0;
      currTime += deltaTime;
      entity = entity >= MAX_ENTITIES ? 0 : entity;
      if (currTime < 0.0166666) {
        return;
      }
      currTime = 0;
      system("clear");
      std::cout << "Entity: "
        << entities[entity]
        << " Position: "
        << Component::Container::getPosition2D(entities[entity], componentData).x
        << " "
        << Component::Container::getPosition2D(entities[entity], componentData).y
        << std::endl;
      ++entity;
    }
    
    void Run()
    {
      Component::Container::Data componentData;
      EntityManager::Data entityData;
      
      EntityManager::InitData(entityData.availableEntities);
      
      std::array<Entity, MAX_ENTITIES> entities;
      
      for (auto& entity : entities) {
        entity = EntityManager::CreateEntity(entityData.availableEntities,
                                             entityData.livingEntityCount);
        Signature sig;
        sig.set(static_cast<size_t>(Component::Type::POSITION2D));
        EntityManager::SetSignature(entityData.signatures,
                                    entity,
                                    sig);
        Component::Container::setPosition2D(entity,
                                            componentData.pos,
                                            {rand() % 999 / 1000.f, rand() % 999 / 1000.f});
      }
      
      float deltaTime = 0;
      while (true)
      {
        auto start = std::chrono::high_resolution_clock::now();
        
        Update(componentData, entityData, entities, deltaTime);
        
        auto stop = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
      }
    }
  }
}
