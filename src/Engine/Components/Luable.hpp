#pragma once

#include "lua.hpp"
#include <filesystem>

namespace Temp::Component::Luable
{
  struct Data
  {
    std::string path{};
#ifdef DEBUG
    std::filesystem::file_time_type time{};
#endif
  };
  
  bool LoadScript(const Data& luable);
  void ExecFunction(const char* luaFn);
}
