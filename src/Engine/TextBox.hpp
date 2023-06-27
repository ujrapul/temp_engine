#pragma once

#include "Entity.hpp"
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include "gl.h"
#endif
#include <string>

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
    std::mutex *mtx{new std::mutex()};
  };
  
  void ConstructRender(Scene::Data &scene, Data *grid);
  void Construct(Scene::Data &scene, Data *grid);
  void UpdateText(Scene::Data &scene, Data *grid, const std::string &newText);
  void UpdateRender(Scene::Data &scene, Data *grid);
  std::tuple<std::vector<float>, std::vector<unsigned int>> PopulateVerticesIndices(Data *grid);
}
