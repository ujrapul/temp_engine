#pragma once

#include "Entity.hpp"
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include "gl.h"
#endif
#include <string>
#include <vector>

namespace Temp::Scene
{
  struct Data;
}

// NOTE: Don't use inline global mutexes, it'll stall multiple instances of the same object
namespace Temp::TextBox
{
  struct Data
  {
    std::string text{};
    float x{};
    float y{};
    float scale{};
    Entity entity{};
    bool renderText{false};
    bool enableOutline{false};
    std::vector<float> vertices{};
    std::vector<unsigned int> indices{};
    std::mutex *mtx{new std::mutex()};
  };
  
  void DrawConstruct(Scene::Data &scene, Data &textBox);
  void Construct(Scene::Data &scene, Data &textBox);
  void UpdateText(Scene::Data &scene, Data &textBox, const std::string &newText);
  void UpdateRender(Scene::Data &scene, Data &textBox);
  void EnableOutline(Scene::Data &scene, Data &textBox, bool enable);
  void DrawDestruct(Scene::Data &scene, Data &textBox);
  void DrawReload(Scene::Data &scene, Data &textBox, int shaderIdx);
  void Destruct(Data &textBox);
}
