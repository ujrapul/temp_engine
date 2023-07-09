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
  
  // TODO: Perhaps add static functions that are called here
  // Every LuaScript should have these functions
  // Or we should provide error checking for whether those fns exist
  bool LoadScript(const Data& luable);
  void ExecFunction(const char* luaFn);
}
