#pragma once

#include "Entity.hpp"
#include "Scene.hpp"
#include "TextBox.hpp"

namespace Temp::TextButton
{
  struct Data
  {
    Entity entity{};
    float width{};
    float height{};
    TextBox::Data textBox{};
  };

  inline void Construct(Scene::Data *scene, Data *data)
  {
    TextBox::Construct(scene, &data->textBox);
    
  }
}