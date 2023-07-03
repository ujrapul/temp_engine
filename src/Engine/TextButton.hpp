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

  inline void Construct(Scene::Data &scene, Data &textButton, Component::Hoverable::Data hoverable)
  {
    textButton.entity = Scene::CreateEntity(scene);
    Scene::AddComponent<Component::Type::HOVERABLE>(scene, textButton.entity, std::move(hoverable));

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
  
  inline void DrawDestruct(Scene::Data &scene, Data &textButton)
  {
    TextBox::DrawDestruct(scene, textButton.textBox);
  }
}
