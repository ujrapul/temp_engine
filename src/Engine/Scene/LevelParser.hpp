#pragma once

#include "Scene.hpp"
#include "EngineUtils.hpp"
#include <fstream>
#include <string>
#include <cerrno>
#include <unordered_map>
#include <vector>

namespace Temp::LevelParser
{
  struct Data
  {
  };

  inline const std::unordered_map<std::string_view, int> parseTable = {
    {"TextBox",    SceneObject::Type::TEXTBOX},
    {"TextButton", SceneObject::Type::TEXTBUTTON},
    {"Max",        SceneObject::Type::MAX}
  };

  // Make sure memory that's used for objects is not leaked!
  bool Parse(Scene::Data& scene, const std::string &file);
} // namespace Temp::LevelParser
