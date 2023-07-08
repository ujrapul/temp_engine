#pragma once

#include "Entity.hpp"
#include "Scene.hpp"
#include "TextBox.hpp"
#include "Hoverable.hpp"

namespace Temp::TextButton
{
  struct Data
  {
    Entity entity{};
    TextBox::Data textBox{};
  };

  // TODO: Define default hover, and enter behavior

  inline void Construct(Scene::Data &scene, Data &textButton, Component::Hoverable::Data hoverable)
  {
    textButton.entity = Scene::CreateEntity(scene);
    hoverable.callbackData = &textButton;
    Scene::AddComponent<Component::Type::HOVERABLE>(scene, textButton.entity, hoverable);
    Scene::AddComponent<Component::Type::LUABLE>(scene, textButton.entity, {"Test.lua"});

    const auto &luable = Scene::Get<Component::Type::LUABLE>(scene, textButton.entity);
    Component::Luable::LoadScript(luable);

    TextBox::Construct(scene, textButton.textBox);
  }

  inline void DrawConstruct(Scene::Data &scene, Data &textButton)
  {
    TextBox::DrawConstruct(scene, textButton.textBox);
  }

  inline void UpdateRender(Scene::Data &scene, Data &textButton)
  {
    TextBox::UpdateRender(scene, textButton.textBox);
  }

  inline void Update(Scene::Data &, Data &, float deltaTime)
  {
    static float time = 0;
    time += deltaTime;
    if (time > 1.f)
    {
      Component::Luable::ExecFunction("myluafunction");
      time = 0;
    }
  }

  inline void DrawDestruct(Scene::Data &scene, Data &textButton)
  {
    TextBox::DrawDestruct(scene, textButton.textBox);
  }

  inline void DrawReload(Scene::Data &scene, Data &textButton, int shaderIdx)
  {
    TextBox::DrawReload(scene, textButton.textBox, shaderIdx);
  }

  inline void Destruct(Data &textButton)
  {
    TextBox::Destruct(textButton.textBox);
  }
  
  inline void HoverEnter(Temp::Scene::Data &scene, Temp::Component::Hoverable::Data &hoverable)
  {
    auto *button = static_cast<TextButton::Data *>(hoverable.callbackData);
    TextBox::EnableOutline(scene, button->textBox, true);
  }

  inline void HoverLeave(Temp::Scene::Data &scene, Temp::Component::Hoverable::Data &hoverable)
  {
    auto *button = static_cast<TextButton::Data *>(hoverable.callbackData);
    TextBox::EnableOutline(scene, button->textBox, false);
  }

  inline void SetHoverableCallbacks(Scene::Data &scene,
                                    Data &textButton,
                                    void (*Click)(Scene::Data &, Component::Hoverable::Data &) = Temp::Component::Hoverable::NoOp,
                                    void (*HoverEnter)(Scene::Data &, Component::Hoverable::Data &) = TextButton::HoverEnter,
                                    void (*HoverLeave)(Scene::Data &, Component::Hoverable::Data &) = TextButton::HoverLeave
                                    )
  {
    auto& hoverable = Scene::Get<Component::Type::HOVERABLE>(scene, textButton.entity);
    hoverable.Click = Click;
    hoverable.HoverEnter = HoverEnter;
    hoverable.HoverLeave = HoverLeave;
  }
  
  inline void SetHoverableCallbacks(Scene::Data &scene,
                                    Data &textButton,
                                    void *callbackData,
                                    void (*Click)(Scene::Data &, Component::Hoverable::Data &) = Temp::Component::Hoverable::NoOp,
                                    void (*HoverEnter)(Scene::Data &, Component::Hoverable::Data &) = TextButton::HoverEnter,
                                    void (*HoverLeave)(Scene::Data &, Component::Hoverable::Data &) = TextButton::HoverLeave
                                    )
  {
    auto& hoverable = Scene::Get<Component::Type::HOVERABLE>(scene, textButton.entity);
    hoverable.Click = Click;
    hoverable.HoverEnter = HoverEnter;
    hoverable.HoverLeave = HoverLeave;
    hoverable.callbackData = callbackData;
  }
}
