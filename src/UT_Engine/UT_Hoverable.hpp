#pragma once

#include "UT_Common.hpp"
#include "Hoverable.hpp"
#include "Scene.hpp"

namespace Temp::Component::Hoverable::UnitTests
{
  bool isHoverEnter = false;
  bool isHoverLeave = false;
  
  void ResetBools()
  {
    isHoverEnter = false;
    isHoverLeave = false;
  }
  
  void HoverEnter(Scene::Data &, Hoverable::Data &)
  {
    isHoverEnter = true;
  }
  
  void HoverLeave(Scene::Data &, Hoverable::Data &)
  {
    isHoverLeave = true;
  }
  
  void Run()
  {
    Scene::Data scene;
    Data hoverable{nullptr, HoverEnter, HoverLeave, nullptr, nullptr, -1000, -1000, 2000, 2000};
    Assert("Test Hoverable is Inside", Hoverable::IsInside(hoverable, 0, 0));
    Assert("Test Hoverable is not Inside", !Hoverable::IsInside(hoverable, -1000, -1000));
    
    Hoverable::HoverableEnter(scene, hoverable);
    Assert("Test Hoverable HoverableEnter", isHoverEnter && hoverable.lastInside);
    
    Hoverable::HoverableLeave(scene, hoverable);
    Assert("Test Hoverable HoverableEnter", isHoverLeave && !hoverable.lastInside);
  }
}
