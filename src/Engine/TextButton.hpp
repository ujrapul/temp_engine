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

  inline void Construct(Scene::Data &scene, Data *data, Component::Hoverable::Data hoverable)
  {
    data->entity = Scene::CreateEntity(scene);
    Scene::AddComponent<Component::Type::HOVERABLE>(scene, data->entity, std::move(hoverable));

    TextBox::Construct(scene, &data->textBox);
  }

  inline void ConstructRender(Scene::Data &scene, Data *data)
  {
    TextBox::ConstructRender(scene, &data->textBox);
  }

  inline void UpdateRender(Scene::Data &scene, Data *data)
  {
    TextBox::UpdateRender(scene, &data->textBox);
  }
}