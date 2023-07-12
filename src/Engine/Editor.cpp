#include "Editor.hpp"
#include "imgui.h"
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
      std::cout << "GOT HERE!" << std::endl;
    }
    ImGui::Button("TextButton");
    ImGui::End();
  }
}