#pragma once

#include "Engine/Scene.hpp"

namespace
{
  const int BOARD_WIDTH = 50;
}

namespace Game
{
  namespace Component
  {
    namespace Type
    {
      const uint8_t VALUE = Temp::Component::Type::MAX;
      const uint8_t MAX = 255;
    }
    
    template <uint8_t> struct MapToComponentDataType_t;
    template <> struct MapToComponentDataType_t<Type::VALUE> { using type = int; };
    
    template <uint8_t T>
    using MapToComponentDataType = typename MapToComponentDataType_t<T>::type;
    
    namespace Container
    {
      void Init(Temp::Component::Container::Data& data)
      {
        Temp::Component::Container::Init<Type::VALUE, MapToComponentDataType<Type::VALUE>>(data);
      }
      
      void Destruct(Temp::Component::Container::Data& data)
      {
        delete Temp::Component::Container::GetComponentArray<Type::VALUE, MapToComponentDataType<Type::VALUE>>(data);
      }
    }
  }
  
  namespace Coordinator
  {
    void Init(Temp::Coordinator::Data& data)
    {
      Temp::Coordinator::Init(data);
      Component::Container::Init(data.componentData);
    }
  }
}

namespace Game
{
  namespace Scene
  {
    namespace GameLevel
    {
      using namespace Temp;
      
      void PrintBoard(Temp::Scene::Data* data, float deltaTime)
      {
        static float currTime = 0;
        currTime += deltaTime;
//        index %= data->coordinator.componentData.positions.mapping.size;
        if (currTime < 0.0166666f) {
          return;
        }
        currTime = 0;
        
        system("clear");

        for (const auto& entity : data->entities) {
          Math::Vec2 pos = Temp::Component::Container::Get<Temp::Component::Type::POSITION2D, Temp::Component::MapToComponentDataType<Temp::Component::Type::POSITION2D>>
            (data->coordinator.componentData, entity);
          std::cout << Temp::Component::Container::Get<Game::Component::Type::VALUE, Component::MapToComponentDataType<Game::Component::Type::VALUE>>
            (data->coordinator.componentData, entity)
                    << " ";
          if (pos.y == BOARD_WIDTH - 1) {
            std::cout << std::endl;
            if (pos.x == BOARD_WIDTH - 1) {
              break;
            }
          }
        }
      }
      
      void Construct(Temp::Scene::Data* data)
      {
        int count = 0;
        int row = 0;
        int column = 0;
        srand(static_cast<uint>(time(NULL)));
        for (auto& entity : data->entities) {
          if (count >= BOARD_WIDTH * BOARD_WIDTH) {
            return;
          }
          entity = Temp::Coordinator::CreateEntity(data->coordinator);
          Temp::Coordinator::AddComponent<Temp::Component::Type::POSITION2D, Temp::Component::MapToComponentDataType<Temp::Component::Type::POSITION2D>>
            (data->coordinator,
             entity,
             Math::Vec2{(float)row, (float)column});
          Temp::Coordinator::AddComponent<Component::Type::VALUE, Component::MapToComponentDataType<Component::Type::VALUE>>
            (data->coordinator,
             entity,
             rand() % 10);
          
          column = ++column % BOARD_WIDTH;
          row = column == 0 ? row + 1 : row;
          ++count;
        }
      }
      
      void Update(Temp::Scene::Data* data, float deltaTime)
      {
        PrintBoard(data, deltaTime);
      }
      
      void Destruct(Temp::Scene::Data* data)
      {
        for (auto& entity : data->entities) {
          Temp::Coordinator::DestroyEntity(data->coordinator, entity);
          entity = 0;
        }
        Component::Container::Destruct(data->coordinator.componentData);
      }
      
      Temp::Scene::Data* Create()
      {
        Temp::Scene::Data* scene = new Temp::Scene::Data();
        Coordinator::Init(scene->coordinator);
        scene->Construct = Construct;
        scene->Update = Update;
        scene->Destruct = Destruct;
        return scene;
      }
    }
  }
}
