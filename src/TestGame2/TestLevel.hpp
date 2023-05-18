#pragma once

#include "Engine/Scene.hpp"

namespace Game
{
  namespace Scene
  {
    namespace TestLevel
    {
      using namespace Temp;
      
      size_t UpdatePositions(Temp::Scene::Data* data,
                             float deltaTime,
                             size_t& index)
      {
        static float currTime = 0;
        currTime += deltaTime;
        index %= Temp::Scene::GetComponentSize<Component::Type::POSITION2D>(*data);
        if (currTime < 0.0166666) {
          return 0;
        }
        currTime = 0;
        
        Entity entity = Temp::Scene::GetEntityUsingIndex<Component::Type::POSITION2D>(*data, index);
        system("clear");
        std::cout << "Entity: "
          << entity
          << " Position: "
          << Temp::Scene::Get<Component::Type::POSITION2D>(*data, entity).x
          << " "
          << Temp::Scene::Get<Component::Type::POSITION2D>(*data, entity).y
          << std::endl;
        ++index;
        
        return index;
      }
      
      void Construct1(Temp::Scene::Data* data)
      {
        for (auto& entity : data->entities) {
          entity = Coordinator::CreateEntity(data->coordinator);
          if (entity % 3 == 0) {
            Temp::Scene::AddComponent<Component::Type::POSITION2D>(*data, entity, Math::Vec2{rand() % 999 / 1000.f, rand() % 999 / 1000.f});
          }
        }
      }
      
      void Construct2(Temp::Scene::Data* data)
      {
        for (auto& entity : data->entities) {
          entity = Coordinator::CreateEntity(data->coordinator);
          if (entity % 5 == 0) {
            Temp::Scene::AddComponent<Component::Type::POSITION2D>(*data, entity, Math::Vec2{rand() % 999 / 1000.f, rand() % 999 / 1000.f});
          }
        }
      }
      
      void Update1(Temp::Scene::Data* data, float deltaTime)
      {
        static size_t index = 0;
        if (UpdatePositions(data, deltaTime, index) >= Temp::Scene::GetComponentSize<Component::Type::POSITION2D>(*data)) {
          data->state = Temp::Scene::State::LEAVE;
        }
      }
      
      void Update2(Temp::Scene::Data* data, float deltaTime)
      {
        static size_t index = 0;
        if (UpdatePositions(data, deltaTime, index) >= Temp::Scene::GetComponentSize<Component::Type::POSITION2D>(*data)) {
          data->state = Temp::Scene::State::LEAVE;
        }
      }
      
      void Destruct(Temp::Scene::Data* data)
      {
        for (auto& entity : data->entities) {
          Coordinator::DestroyEntity(data->coordinator, entity);
          entity = 0;
        }
      }
      
      Temp::Scene::Data* Create1()
      {
        Temp::Scene::Data* scene = new Temp::Scene::Data();
        Coordinator::Init(scene->coordinator);
        scene->Construct = Construct1;
        scene->Update = Update1;
        scene->Destruct = Destruct;
        return scene;
      }
      
      Temp::Scene::Data* Create2()
      {
        Temp::Scene::Data* scene = new Temp::Scene::Data();
        Coordinator::Init(scene->coordinator);
        scene->Construct = Construct2;
        scene->Update = Update2;
        scene->Destruct = Destruct;
        return scene;
      }
    }
  }
}
