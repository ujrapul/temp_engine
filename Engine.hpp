#include "Components/ComponentContainer.hpp"
#include "Components/ComponentType.hpp"
#include "EntityManager.hpp"
#include <iostream>
#include <chrono>

namespace Temp
{
  namespace Coordinator
  {
    struct Data
    {
      EntityManager::Data entityData;
      Component::Container::Data componentData;
    };
    
    Entity CreateEntity(Data& data)
    {
      return EntityManager::CreateEntity(data.entityData.availableEntities,
                                         data.entityData.livingEntityCount);
    }
    
    void DestroyEntity(Data& data, Entity entity)
    {
      Component::Container::EntityDestroyed(data.componentData, entity);
      EntityManager::DestroyEntity(data.entityData.availableEntities,
                                   data.entityData.livingEntityCount,
                                   data.entityData.signatures,
                                   entity);
    }
    
    void Init(Data& data)
    {
      EntityManager::InitData(data.entityData.availableEntities);
      Component::Container::Init(data.componentData);
    }
    
    template<typename T>
    void AddComponent(Data& data, Entity entity, T component, Component::Type type)
    {
      Signature sig = EntityManager::GetSignature(data.entityData.signatures, entity);
      sig.set(static_cast<size_t>(type));
      EntityManager::SetSignature(data.entityData.signatures,
                                  entity,
                                  sig);
      Component::Container::setPosition2D(data.componentData.positions,
                                          entity,
                                          component);
    }
    
    template<typename T>
    void UpdateComponent(Data& data, Entity entity, T component, Component::Type type)
    {
      if (EntityManager::GetSignature(data.entityData.signatures, entity).test(static_cast<size_t>(type)))
        Component::Container::Get<T>(data, entity) = component;
    }
  }
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

  namespace Engine
  {
    struct Data
    {
      std::vector<Scene::Data*> scenes;
    };
    
    void Run()
    {
      Engine::Data engine;
      Scene::Data* scene1 = Scene::TestLevel::Create1();
      Scene::Data* scene2 = Scene::TestLevel::Create2();
      scene1->nextScene = scene2;
      scene2->nextScene = scene1;
      engine.scenes.push_back(scene1);
      engine.scenes.push_back(scene2);
      Scene::Data* currentScene = engine.scenes.front();
      
      float deltaTime = 0;
      while (true)
      {
        auto start = std::chrono::high_resolution_clock::now();
        
        switch (currentScene->state) {
          case Scene::State::ENTER:
            if (currentScene->Construct)
              currentScene->Construct(currentScene);
            currentScene->state = Scene::State::RUN;
            break;
          case Scene::State::RUN:
            if (currentScene->Update)
              currentScene->Update(currentScene, deltaTime);
            break;
          case Scene::State::LEAVE:
            if (currentScene->Destruct)
              currentScene->Destruct(currentScene);
            currentScene->state = Scene::State::ENTER;
            currentScene = currentScene->nextScene;
            currentScene->state = Scene::State::ENTER;
            break;
          default:
            break;
        }

        auto stop = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
      }
      
      delete scene1;
      delete scene2;
    }
  }
}
