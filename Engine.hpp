#include "Scene.hpp"
#include <chrono>

namespace Temp
{
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
