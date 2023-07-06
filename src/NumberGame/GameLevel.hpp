#pragma once

#include "Engine/Scene/Scene.hpp"
#include "Engine/Input.hpp"

namespace Game::Scene::GameLevel
{
  using namespace Temp;

  Temp::Scene::SceneFns Create(Temp::Input::KeyEventData &keyEventData);
  Temp::Scene::SceneFns Create2(Temp::Input::KeyEventData &keyEventData);
}
