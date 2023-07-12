#pragma once

#include "UT_Common.hpp"
#include "Event.hpp"

namespace Temp::Event::UnitTests
{
  bool isHoverEnter = false;
  bool isHoverLeave = false;
  bool isButtonReleased = false;
  
  void ResetBools()
  {
    isHoverEnter = false;
    isHoverLeave = false;
    isButtonReleased = false;
  }
  
  void HoverEnter(Scene::Data &, Component::Hoverable::Data &)
  {
    isHoverEnter = true;
  }
  
  void HoverLeave(Scene::Data &, Component::Hoverable::Data &)
  {
    isHoverLeave = true;
  }
  
  void ButtonReleased(Scene::Data &, Component::Hoverable::Data &)
  {
    isButtonReleased = true;
  }

  void Run()
  {
    Event::Hover(0, 0);
    Event::ButtonReleased(0, 0, 1);
    Assert("Test Event Hover Terminates with No Scene", !isHoverEnter && !isHoverLeave);
    Assert("Test Event ButtonReleased Terminates with No Scene", !isButtonReleased);
    
    Scene::Data scene;
    Scene::Construct(scene);
    Coordinator::Init(scene.coordinator);
    Temp::Engine::engine.currentScene = &scene;
    
    Event::Hover(0, 0);
    Event::ButtonReleased(0, 0, 1);

    Assert("Test Event Hover Terminates with non Scene State Run", !isHoverEnter && !isHoverLeave);
    Assert("Test Event ButtonReleased Terminates with non Scene State Run", !isButtonReleased);
    
    scene.state = Scene::State::RUN;

    Event::Hover(0, 0);
    Event::ButtonReleased(0, 0, 1);
    
    Assert("Test Event Hover Terminates with No Hoverable", !isHoverEnter && !isHoverLeave);
    Assert("Test Event ButtonReleased Terminates with No Hoverable", !isButtonReleased);
    
    Scene::AddComponent<Component::Type::HOVERABLE>(scene, 0, {});
    
    Event::Hover(0, 0);
    Event::ButtonReleased(0, 0, 1);
    
    Assert("Test Event Hover Terminates with No Hoverable Callback", !isHoverEnter && !isHoverLeave);
    Assert("Test Event ButtonReleased Terminates with No Hoverable Callback", !isButtonReleased);
    
    Scene::AddComponent<Component::Type::HOVERABLE>(scene, 0,
                                                    {ButtonReleased, HoverEnter, HoverLeave, nullptr, nullptr, -1280, -720, 9999, 9999});
    
    Event::Hover(0, 0);
    Event::ButtonReleased(0, 0, 1);
    
    Assert("Test Event Hover Enter when Mouse in Hoverable", isHoverEnter && !isHoverLeave);
    Assert("Test Event ButtonReleased when Mouse in Hoverable", isButtonReleased);
    
    ResetBools();
    
    Scene::AddComponent<Component::Type::HOVERABLE>(scene, 0,
                                                    {ButtonReleased, HoverEnter, HoverLeave, nullptr, nullptr, -9999, -9999, 0, 0, true});
    
    Event::Hover(0, 0);
    Event::ButtonReleased(0, 0, 1);
    
    Assert("Test Event Hover Leave when Mouse is not in Hoverable", !isHoverEnter && isHoverLeave);
    Assert("Test Event ButtonReleased when Mouse is not in Hoverable", !isButtonReleased);
    
    Scene::Destruct(scene);
    Coordinator::Destruct(scene.coordinator);
  }
}
