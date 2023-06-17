#pragma once

#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Input.hpp"
#include "GameLevel.hpp"

namespace Game
{
  namespace Entry
  {
    Temp::Engine::Data engine{};

    void Exit(Temp::Input::KeyboardCode /*keyCode*/)
    {
      Temp::Engine::Quit(engine);
    }

    void Run(const char *windowName)
    {
      using namespace Temp;

      Camera::SetProjection(Camera::Projection::ORTHOGRAPHIC);

      Math::UnitTests::Run();

      Engine::Construct(engine);
      Temp::Scene::Data *scene1 = Scene::GameLevel::Create(engine.keyEventData);
      Temp::Scene::Data *scene2 = Scene::GameLevel::Create2(engine.keyEventData);
      scene1->nextScene = scene2;
      scene2->nextScene = scene1;
      engine.scenes.push_back(scene1);
      engine.scenes.push_back(scene2);

      // Testing Add and Remove works
      Input::AddCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      Input::RemoveCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      Input::AddCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);

      Temp::Engine::Run(engine, windowName, 1280, 720);

      Temp::Engine::Destroy(engine);
    }
  }
}
