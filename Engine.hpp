#include "Components/ComponentContainer.hpp"
#include "Components/ComponentType.hpp"
#include "EntityManager.hpp"
#include <iostream>
#include <chrono>

// TODO: Add events for when entity is destroyed
//       Function pointers should work for this
//       Maybe change states to Enter, Run, Leave???

namespace Temp
{
  namespace Scene
  {
    enum class State : uint8_t
    {
      CONSTRUCTION = 0,
      RUNNING      = 1,
      DESTRUCTION  = 2,
    };
    
    struct Data
    {
      EntityManager::Data entityData;
      Component::Container::Data componentData;
      std::array<Entity, MAX_ENTITIES> entities;
      State state{State::CONSTRUCTION};
      Data* nextScene{nullptr};
      void (*Construct)(Scene::Data*){nullptr};
      void (*Update)(Scene::Data*, float){nullptr};
      void (*Destruct)(Scene::Data*){nullptr};
    };
    
    namespace TestLevel
    {
      size_t UpdatePositions(Component::ArrayData<Math::Vec2>& positions,
                             float deltaTime,
                             size_t& index)
      {
        static float currTime = 0;
        currTime += deltaTime;
        index %= positions.mapping.size;
        if (currTime < 0.0166666) {
          return 0;
        }
        currTime = 0;

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
        
        return index;
      }
      
      void Construct1(Scene::Data* data)
      {
        for (auto& entity : data->entities) {
          entity = EntityManager::CreateEntity(data->entityData.availableEntities,
                                               data->entityData.livingEntityCount);
          Signature sig;
          if (entity % 3 == 0) {
            sig.set(static_cast<size_t>(Component::Type::POSITION2D));
            EntityManager::SetSignature(data->entityData.signatures,
                                        entity,
                                        sig);
            Component::Container::setPosition2D(data->componentData.positions,
                                                entity,
                                                {rand() % 999 / 1000.f, rand() % 999 / 1000.f});
          }
        }
      }
      
      void Construct2(Scene::Data* data)
      {
        for (auto& entity : data->entities) {
          entity = EntityManager::CreateEntity(data->entityData.availableEntities,
                                               data->entityData.livingEntityCount);
          Signature sig;
          if (entity % 5 == 0) {
            sig.set(static_cast<size_t>(Component::Type::POSITION2D));
            EntityManager::SetSignature(data->entityData.signatures,
                                        entity,
                                        sig);
            Component::Container::setPosition2D(data->componentData.positions,
                                                entity,
                                                {rand() % 999 / 1000.f, rand() % 999 / 1000.f});
          }
        }
      }
      
      void Update1(Scene::Data* data, float deltaTime)
      {
        static size_t index = 0;
        if (UpdatePositions(data->componentData.positions, deltaTime, index) >= data->componentData.positions.mapping.size) {
          data->state = State::DESTRUCTION;
        }
      }
      
      void Update2(Scene::Data* data, float deltaTime)
      {
        static size_t index = 0;
        if (UpdatePositions(data->componentData.positions, deltaTime, index) >= data->componentData.positions.mapping.size) {
          data->state = State::DESTRUCTION;
        }
      }
      
      void Destruct(Scene::Data* data)
      {
        for (auto& entity : data->entities) {
          if (EntityManager::GetSignature(data->entityData.signatures, entity).test(static_cast<size_t>(Component::Type::POSITION2D))) {
            Component::Remove(data->componentData.positions, entity);
          }
          EntityManager::DestroyEntity(data->entityData.availableEntities,
                                       data->entityData.livingEntityCount,
                                       data->entityData.signatures,
                                       entity);
          entity = 0;
        }
      }
      
      Data* Create1()
      {
        Data* scene = new Data();
        EntityManager::InitData(scene->entityData.availableEntities);
        Component::Container::Init(scene->componentData);
        scene->Construct = Construct1;
        scene->Update = Update1;
        scene->Destruct = Destruct;
        return scene;
      }
      
      Data* Create2()
      {
        Data* scene = new Data();
        EntityManager::InitData(scene->entityData.availableEntities);
        Component::Container::Init(scene->componentData);
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
          case Scene::State::CONSTRUCTION:
            if (currentScene->Construct)
              currentScene->Construct(currentScene);
            currentScene->state = Scene::State::RUNNING;
            break;
          case Scene::State::RUNNING:
            if (currentScene->Update)
              currentScene->Update(currentScene, deltaTime);
            break;
          case Scene::State::DESTRUCTION:
            if (currentScene->Destruct)
              currentScene->Destruct(currentScene);
            currentScene->state = Scene::State::CONSTRUCTION;
            currentScene = currentScene->nextScene;
            currentScene->state = Scene::State::CONSTRUCTION;
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
