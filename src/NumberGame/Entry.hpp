#pragma once

#include "GameLevel.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Input.hpp"

namespace Game
{
  namespace Entry
  {
    Temp::Engine::Data engine{};

    void Exit(Temp::Input::KeyboardCode /*keyCode*/)
    {
      Temp::Engine::Quit(engine);
    }
    
    void Run(const char* windowName)
    {
      using namespace Temp;

      Component::Drawable::SetProjection(Component::Drawable::Projection::ORTHOGRAPHIC);

      Math::UnitTests::Run();
      
      engine = Engine::Construct();
      Temp::Scene::Data* scene1 = Scene::GameLevel::Create(engine.keyEventData);
//      Temp::Scene::Data* scene2 = Scene::GameLevel::Create2();
//      scene1->nextScene = scene2;
//      scene2->nextScene = scene1;
      engine.scenes.push_back(scene1);
//      engine.scenes.push_back(scene2);
      
      // Testing Add and Remove works
      Input::AddCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      Input::RemoveCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      Input::AddCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);

      Temp::Engine::Run(engine, windowName);
      
      Temp::Engine::Destroy(engine);
    }
  }
}
