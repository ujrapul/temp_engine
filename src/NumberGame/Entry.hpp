#pragma once

#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Input.hpp"
#include "GameLevel.hpp"
#include "MainMenuLevel.hpp"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

extern "C" {
  static int l_cppfunction(lua_State *L) {
    double arg = luaL_checknumber(L,1);
    lua_pushnumber(L, arg * 0.5);
    return 1;
  }
}

namespace Game
{
  namespace Entry
  {
    void Exit(Temp::Input::KeyboardCode /*keyCode*/)
    {
      Temp::Engine::Quit(Temp::Engine::engine);
    }

    void Run(const char *windowName)
    {
      using namespace std;
      using namespace Temp;

      Camera::SetProjection(Camera::Projection::ORTHOGRAPHIC);

      auto &engine = Temp::Engine::engine;

      Engine::Construct(engine);
      Temp::Scene::Data *scene1 = Scene::GameLevel::Create(engine.keyEventData);
      Temp::Scene::Data *scene2 = Scene::GameLevel::Create2(engine.keyEventData);
      Temp::Scene::Data *mainMenuLevel = Scene::MainMenuLevel::Create();
      mainMenuLevel->nextScene = scene1;
      scene1->nextScene = scene2;
      scene2->nextScene = scene1;
      engine.scenes.push_back(mainMenuLevel);
      engine.scenes.push_back(scene1);
      engine.scenes.push_back(scene2);

      // Testing Add and Remove works
      Input::AddCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      Input::RemoveCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      Input::AddCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      
      cout << "** Test Lua embedding" << endl;
      cout << "** Init Lua" << endl;
      lua_State *L;
      L = luaL_newstate();
      cout << "** Load the (optional) standard libraries, to have the print function" << endl;
      luaL_openlibs(L);
      cout << "** Load chunk. without executing it" << endl;
      if (luaL_loadfile(L, "LuaScripts/Test.lua")) {
        cerr << "Something went wrong loading the chunk (syntax error?)" << endl;
        cerr << lua_tostring(L, -1) << endl;
        lua_pop(L,1);
      }

      cout << "** Make a insert a global var into Lua from C++" << endl;
      lua_pushnumber(L, 1.1);
      lua_setglobal(L, "cppvar");

      cout << "** Execute the Lua chunk" << endl;
      if (lua_pcall(L,0, LUA_MULTRET, 0)) {
        cerr << "Something went wrong during execution" << endl;
        cerr << lua_tostring(L, -1) << endl;
        lua_pop(L,1);
      }
      
      cout << "** Read a global var from Lua into C++" << endl;
      lua_getglobal(L, "luavar");
      double luavar = lua_tonumber(L,-1);
      lua_pop(L,1);
      cout << "C++ can read the value set from Lua luavar = " << luavar << endl;

      cout << "** Execute a Lua function from C++" << endl;
      lua_getglobal(L, "myluafunction");
      lua_pushnumber(L, 5);
      lua_pcall(L, 1, 1, 0);
      cout << "The return value of the function was " << lua_tostring(L, -1) << endl;
      lua_pop(L,1);

      cout << "** Execute a C++ function from Lua" << endl;
      cout << "**** First register the function in Lua" << endl;
      lua_pushcfunction(L,l_cppfunction);
      lua_setglobal(L, "cppfunction");

      cout << "**** Call a Lua function that uses the C++ function" << endl;
      lua_getglobal(L, "myfunction");
      lua_pushnumber(L, 5);
      lua_pcall(L, 1, 1, 0);
      cout << "The return value of the function was " << lua_tonumber(L, -1) << endl;
      lua_pop(L,1);

      Temp::Engine::Run(engine, windowName, 1280, 720);
      
      cout << "** Release the Lua enviroment" << endl;
      lua_close(L);
    }
  }
}
