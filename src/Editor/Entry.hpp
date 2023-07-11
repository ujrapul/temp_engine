#pragma once

#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Input.hpp"
#include "EditorLevel.hpp"
#include "Coordinator.hpp"

namespace Temp
{
  namespace Entry
  {
    void Run(const char *windowName)
    {
      using namespace std;
      
      Camera::SetProjection(Camera::Projection::ORTHOGRAPHIC);
      
      auto &engine = Temp::Engine::engine;
      
      Engine::Construct(engine);
      Scene::SceneFns scene = Scene::EditorLevel::Create();
      engine.sceneFns.push_back(scene);
      engine.coordinatorFns.Init = Coordinator::Init;
      engine.coordinatorFns.Destruct = Coordinator::Destruct;
      engine.coordinatorFns.Reset = Coordinator::Reset;
      
      Engine::Run(engine, windowName, 1920, 1080);
    }
  }
}
