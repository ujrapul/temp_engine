#pragma once

#include "Engine/Scene.hpp"

namespace
{
  const int BOARD_WIDTH = 10;
}

namespace Game
{
  namespace Component
  {
    enum class Type : uint8_t
    {
      VALUE = static_cast<uint8_t>(Temp::Component::Type::MAX),
      MAX = 255,
    };
    
    template <Type> struct MapToComponentDataType_t;
    template <> struct MapToComponentDataType_t<Type::VALUE> { using type = int; };
    
    template <Type T>
    using MapToComponentDataType = typename MapToComponentDataType_t<T>::type;

    namespace Container
    {
      void Init(Temp::Component::Container::Data& data)
      {
        Temp::Component::Container::Init<MapToComponentDataType<Type::VALUE>>
          (data, static_cast<uint8_t>(Type::VALUE));
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
          std::cout << "Entity: "
          << entity
          << " Position: "
          << Temp::Component::Container::Get<Math::Vec2>(data->coordinator.componentData,
                                                         entity,
                                                         Temp::Component::Type::POSITION2D).x
          << " "
          << Temp::Component::Container::Get<Math::Vec2>(data->coordinator.componentData,
                                                         entity,
                                                         Temp::Component::Type::POSITION2D).y
          << std::endl;
        }
      }
      
      void Construct(Temp::Scene::Data* data)
      {
        int count = 0;
        int row = 0;
        int column = 0;
        for (auto& entity : data->entities) {
          if (count >= BOARD_WIDTH * BOARD_WIDTH) {
            return;
          }
          entity = Temp::Coordinator::CreateEntity(data->coordinator);
          Temp::Coordinator::AddComponent(data->coordinator,
                                          entity,
                                          Math::Vec2{(float)row, (float)column},
                                          Temp::Component::Type::POSITION2D);
          
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
