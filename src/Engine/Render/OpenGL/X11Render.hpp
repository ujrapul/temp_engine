#pragma once

#include "Engine.hpp"

namespace Temp::Render
{
  void Initialize(const char *windowName, Engine::Data &engine);
  void Run(Engine::Data& engine);
  void Destroy();
}
