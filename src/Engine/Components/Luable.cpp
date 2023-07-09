#include "Luable.hpp"
#include "Engine.hpp"
#include "Logger.hpp"
#include "EngineUtils.hpp"
#include <atomic>

namespace Temp::Component::Luable
{  
  bool LoadScript(const Data& luable)
  {
    auto* L = Engine::engine.lua;
    if (luaL_loadfile(L, (AssetsDirectory() / "LuaScripts" / luable.path).c_str())) {
      Logger::Log("Something went wrong loading the chunk (syntax error?)");
      Logger::Log(lua_tostring(L, -1));
      lua_pop(L, 1);
      return false;
    }
    if (lua_pcall(L,0, LUA_MULTRET, 0)) {
      Logger::Log("Something went wrong during execution");
      Logger::Log(lua_tostring(L, -1));
      lua_pop(L,1);
      return false;
    }
    return true;
  }
  
  void ExecFunction(const char* luaFn)
  {
    auto* L = Engine::engine.lua;
    lua_getglobal(L, luaFn);
    lua_pushnumber(L, 5);
    lua_pcall(L, 1, 1, 0);
    lua_pop(L,1);
  }
}
