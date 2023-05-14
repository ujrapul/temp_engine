#pragma once

#include "TestLevel.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Input.hpp"

namespace Game
{
  namespace Entry
  {
    void Exit()
    {
      exit(0);
    }
    
    void Run()
    {
      using namespace Temp;
      
      Engine::Data engine;
      Temp::Scene::Data* scene1 = Scene::TestLevel::Create1();
      Temp::Scene::Data* scene2 = Scene::TestLevel::Create2();
      scene1->nextScene = scene2;
      scene2->nextScene = scene1;
      engine.scenes.push_back(scene1);
      engine.scenes.push_back(scene2);
      
      // Testing Add and Remove works
      Input::AddCallback(Exit, engine.keyEventData, 12);
      Input::RemoveCallback(Exit, engine.keyEventData, 12);
      Input::AddCallback(Exit, engine.keyEventData, 12);

      Temp::Engine::Run(engine);
      
      Temp::Engine::Destroy(engine);
    }
  }
}
