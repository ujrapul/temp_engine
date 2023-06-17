#pragma once

#include "Engine.hpp"

namespace Temp::Render
{
  void Initialize(const char *windowName, Engine::Data &engine);
  bool IsInitialized();
  void Run(Engine::Data& engine);
  void Destroy();
}
