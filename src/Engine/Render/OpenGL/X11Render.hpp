#pragma once

#include "Engine.hpp"

namespace Temp::Render
{
  void Initialize(const char *windowName, int windowX, int windowY, Engine::Data &engine);
  void Run(Engine::Data& engine);
  void Destroy();
}
