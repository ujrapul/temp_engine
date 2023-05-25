#include "Engine.hpp"
#include "Scene.hpp"
#include <chrono>
#include <thread>

namespace Temp
{
  namespace Engine
  {    
    void Run(Engine::Data& engine)
    {
      Scene::Data* currentScene = engine.scenes.front();      
      
      std::thread inputThread(Input::HandleThread, std::ref(engine.keyEventData));
      inputThread.detach();
      
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
        
        Input::Process(engine.keyEventData);

        auto stop = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
      }
    }
    
    void Destroy(Engine::Data& engine)
    {
      for (Scene::Data* scene : engine.scenes) {
        Scene::Destruct(*scene);
        delete scene;
      }
      engine.scenes.clear();
    }
    
    Engine::Data Construct()
    {
      Engine::Data out;
      out.keyEventData = std::move(Input::Construct());
      return out;
    }
  }
}
