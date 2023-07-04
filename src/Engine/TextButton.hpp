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
    Scene::AddComponent<Component::Type::LUABLE>(scene, textButton.entity, {"LuaScripts/Test.lua"});
    
    const auto& luable = Scene::Get<Component::Type::LUABLE>(scene, textButton.entity);
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
  
  inline void Update(float deltaTime)
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
}
