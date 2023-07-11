#pragma once

#include "Event.hpp"

namespace Temp::Event::UnitTests
{
  bool isHoverEnter = false;
  bool isHoverLeave = false;
  bool isClick = false;
  
  void ResetBools()
  {
    isHoverEnter = false;
    isHoverLeave = false;
    isClick = false;
  }
  
  void HoverEnter(Scene::Data &, Component::Hoverable::Data &)
  {
    isHoverEnter = true;
  }
  
  void HoverLeave(Scene::Data &, Component::Hoverable::Data &)
  {
    isHoverLeave = true;
  }
  
  void Click(Scene::Data &, Component::Hoverable::Data &)
  {
    isClick = true;
  }

  void Run()
  {
    Event::Hover(0, 0);
    Event::Click(0, 0);
    Assert("Test Event Hover Terminates with No Scene", !isHoverEnter && !isHoverLeave);
    Assert("Test Event Click Terminates with No Scene", !isClick);
    
    Scene::Data scene;
    Scene::Construct(scene);
    Coordinator::Init(scene.coordinator);
    Temp::Engine::engine.currentScene = &scene;
    
    Event::Hover(0, 0);
    Event::Click(0, 0);

    Assert("Test Event Hover Terminates with non Scene State Run", !isHoverEnter && !isHoverLeave);
    Assert("Test Event Click Terminates with non Scene State Run", !isClick);
    
    scene.state = Scene::State::RUN;

    Event::Hover(0, 0);
    Event::Click(0, 0);
    
    Assert("Test Event Hover Terminates with No Hoverable", !isHoverEnter && !isHoverLeave);
    Assert("Test Event Click Terminates with No Hoverable", !isClick);
    
    Scene::AddComponent<Component::Type::HOVERABLE>(scene, 0, {});
    
    Event::Hover(0, 0);
    Event::Click(0, 0);
    
    Assert("Test Event Hover Terminates with No Hoverable Callback", !isHoverEnter && !isHoverLeave);
    Assert("Test Event Click Terminates with No Hoverable Callback", !isClick);
    
    Scene::AddComponent<Component::Type::HOVERABLE>(scene, 0,
                                                    {Click, HoverEnter, HoverLeave, nullptr, -1280, -720, 9999, 9999});
    
    Event::Hover(0, 0);
    Event::Click(0, 0);
    
    Assert("Test Event Hover Enter when Mouse in Hoverable", isHoverEnter && !isHoverLeave);
    Assert("Test Event Click when Mouse in Hoverable", isClick);
    
    ResetBools();
    
    Scene::AddComponent<Component::Type::HOVERABLE>(scene, 0,
                                                    {Click, HoverEnter, HoverLeave, nullptr, -9999, -9999, 0, 0, true});
    
    Event::Hover(0, 0);
    Event::Click(0, 0);
    
    Assert("Test Event Hover Leave when Mouse is not in Hoverable", !isHoverEnter && isHoverLeave);
    Assert("Test Event Click when Mouse is not in Hoverable", !isClick);
    
    Scene::Destruct(scene);
    Coordinator::Destruct(scene.coordinator);
  }
}
