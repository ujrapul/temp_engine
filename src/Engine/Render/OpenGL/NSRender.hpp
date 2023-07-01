#pragma once

#include "Engine.hpp"
#include "Math.hpp"

namespace Temp::Render
{
  void Initialize(const char *windowName, int windowX, int windowY, Engine::Data &engine);
  void Run(Engine::Data &engine, const char *windowName, int windowX, int windowY);
  void Run(Engine::Data &engine);
  void Destroy();
  Math::Vec2f GetWindowOrigin();
  Math::Vec2f GetWindowSize();
  Math::Vec2f GetScreenSize();
}
