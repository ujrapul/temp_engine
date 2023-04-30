#include "Components/ComponentContainer.hpp"
#include "Components/ComponentType.hpp"
#include "EntityManager.hpp"
#include <iostream>
#include <chrono>

namespace Temp
{
  namespace Engine
  {
    void UpdatePositions(Component::ArrayData<Math::Vec2>& positions,
//                         EntityManager::Data* entityData,
//                         std::array<Entity, MAX_ENTITIES>& entities,
                         float deltaTime)
    {
      static float currTime = 0;
      static size_t index = 0;
      currTime += deltaTime;
      index %= MAX_ENTITIES;
      if (currTime < 0.0166666) {
        return;
      }
      currTime = 0;
//      if (!EntityManager::GetSignature(entityData->signatures, entity)
//          .test(static_cast<size_t>(Component::Type::POSITION2D)))
//      {
//        return;
//      }
      Entity entity = positions.mapping.indexToEntity[index];
      system("clear");
      std::cout << "Entity: "
        << entity
        << " Position: "
        << Component::Container::getPosition2D(entity, positions).x
        << " "
        << Component::Container::getPosition2D(entity, positions).y
        << std::endl;
      ++index;
    }
    
    void Run()
    {
      Component::Container::Data* componentData = new Component::Container::Data();
      EntityManager::Data* entityData = new EntityManager::Data();
            
      EntityManager::InitData(entityData->availableEntities);
      Component::Container::Init(componentData);
      
      std::array<Entity, MAX_ENTITIES> entities;
      
      for (auto& entity : entities) {
        entity = EntityManager::CreateEntity(entityData->availableEntities,
                                             entityData->livingEntityCount);
        Signature sig;
        if (entity % 3 == 0) {
          sig.set(static_cast<size_t>(Component::Type::POSITION2D));
          EntityManager::SetSignature(entityData->signatures,
                                      entity,
                                      sig);
          Component::Container::setPosition2D(componentData->positions,
                                              entity,
                                              {rand() % 999 / 1000.f, rand() % 999 / 1000.f});
        }
      }
      
      float deltaTime = 0;
      while (true)
      {
        auto start = std::chrono::high_resolution_clock::now();
        
//        UpdatePositions(componentData, entities, deltaTime);
        UpdatePositions(componentData->positions, deltaTime);
        
        auto stop = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
      }
      
      delete(componentData);
      delete(entityData);
    }
  }
}
