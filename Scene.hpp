#pragma once

#include "Coordinator.hpp"
#include <iostream>

namespace Temp
{
  namespace Scene
  {
    enum class State : uint8_t
    {
      ENTER = 0,
      RUN   = 1,
      LEAVE = 2,
    };
    
    struct Data
    {
      Coordinator::Data coordinator;
      std::array<Entity, MAX_ENTITIES> entities;
      State state{State::ENTER};
      Data* nextScene{nullptr};
      void (*Construct)(Scene::Data*){nullptr};
      void (*Update)(Scene::Data*, float){nullptr};
      void (*Destruct)(Scene::Data*){nullptr};
    };
    
    namespace TestLevel
    {
      size_t UpdatePositions(Scene::Data* data,
                             float deltaTime,
                             size_t& index)
      {
        static float currTime = 0;
        currTime += deltaTime;
        index %= data->coordinator.componentData.positions.mapping.size;
        if (currTime < 0.0166666) {
          return 0;
        }
        currTime = 0;
        
        Entity entity = data->coordinator.componentData.positions.mapping.indexToEntity[index];
        system("clear");
        std::cout << "Entity: "
        << entity
        << " Position: "
        << Component::Container::getPosition2D(entity, data->coordinator.componentData.positions).x
        << " "
        << Component::Container::getPosition2D(entity, data->coordinator.componentData.positions).y
        << std::endl;
        ++index;
        
        return index;
      }
      
      void Construct1(Scene::Data* data)
      {
        for (auto& entity : data->entities) {
          entity = Coordinator::CreateEntity(data->coordinator);
          if (entity % 3 == 0) {
            Coordinator::AddComponent(data->coordinator, entity, Math::Vec2{rand() % 999 / 1000.f, rand() % 999 / 1000.f}, Component::Type::POSITION2D);
          }
        }
      }
      
      void Construct2(Scene::Data* data)
      {
        for (auto& entity : data->entities) {
          entity = Coordinator::CreateEntity(data->coordinator);
          if (entity % 5 == 0) {
            Coordinator::AddComponent(data->coordinator, entity, Math::Vec2{rand() % 999 / 1000.f, rand() % 999 / 1000.f}, Component::Type::POSITION2D);
          }
        }
      }
      
      void Update1(Scene::Data* data, float deltaTime)
      {
        static size_t index = 0;
        if (UpdatePositions(data, deltaTime, index) >= data->coordinator.componentData.positions.mapping.size) {
          data->state = State::LEAVE;
        }
      }
      
      void Update2(Scene::Data* data, float deltaTime)
      {
        static size_t index = 0;
        if (UpdatePositions(data, deltaTime, index) >= data->coordinator.componentData.positions.mapping.size) {
          data->state = State::LEAVE;
        }
      }
      
      void Destruct(Scene::Data* data)
      {
        for (auto& entity : data->entities) {
          Coordinator::DestroyEntity(data->coordinator, entity);
          entity = 0;
        }
      }
      
      Data* Create1()
      {
        Data* scene = new Data();
        Coordinator::Init(scene->coordinator);
        scene->Construct = Construct1;
        scene->Update = Update1;
        scene->Destruct = Destruct;
        return scene;
      }
      
      Data* Create2()
      {
        Data* scene = new Data();
        Coordinator::Init(scene->coordinator);
        scene->Construct = Construct2;
        scene->Update = Update2;
        scene->Destruct = Destruct;
        return scene;
      }
    }
  }
}
