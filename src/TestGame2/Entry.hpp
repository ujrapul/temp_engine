#pragma once

#include "TestLevel.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Input.hpp"

namespace Game
{
  namespace Entry
  {
    void Exit(Temp::Input::KeyboardCode /*keyCode*/)
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
      Input::AddCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      Input::RemoveCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      Input::AddCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);

      Temp::Engine::Run(engine, "TestGame2");
      
      Temp::Engine::Destroy(engine);
    }
  }
}
