#include "Editor.hpp"
#include "imgui.h"
#include "Engine.hpp"
#include "TextBox.hpp"
#include "Math.hpp"
#include <iostream>

namespace Temp::Editor
{
  void RunImGUI()
  {
    // Add your ImGui UI code here
    bool show_demo_window;
    ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Begin("Create Entity");
    if (ImGui::Button("TextBox"))
    {
      static int count = 0;
      static float x = 0;
      static float y = 0;
      auto& engine = Engine::engine;

      std::lock_guard<std::mutex> lock1(engine.mtx);
      auto textBox = new TextBox::Data{"Default Text", {}, x, y, 0.5f};
      {
        std::lock_guard<std::mutex> lock2(engine.currentScene->mtx);
        
        TextBox::Construct(*engine.currentScene, *textBox);
        TextBox::DrawConstruct(*engine.currentScene, *textBox);
      }
      
      SceneObject::Data object{textBox, nullptr, "TextObject" + std::to_string(count), SceneObject::Type::TEXTBOX};
      Scene::AddObject(*engine.currentScene, object);

      x += 10;
      y -= 10;
      ++count;
    }
    ImGui::Button("TextButton");
    ImGui::End();
  }
}
