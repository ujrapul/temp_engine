#pragma once

#include "Engine/Scene.hpp"
#include "Engine/Input.hpp"

namespace Game::Scene::GameLevel
{
  using namespace Temp;

  Temp::Scene::Data *Create(Temp::Input::KeyEventData &keyEventData);
  Temp::Scene::Data *Create2(Temp::Input::KeyEventData &keyEventData);
}
