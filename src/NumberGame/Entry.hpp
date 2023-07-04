#pragma once

#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Input.hpp"
#include "GameLevel.hpp"
#include "MainMenuLevel.hpp"
#include "Coordinator.hpp"

//#include "lua.hpp"

//extern "C" {
//  static int l_cppfunction(lua_State *L) {
//    double arg = luaL_checknumber(L,1);
//    lua_pushnumber(L, arg * 0.5);
//    return 1;
//  }
//}

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
      Temp::Scene::SceneFns scene1 = Scene::GameLevel::Create(engine.keyEventData);
      Temp::Scene::SceneFns scene2 = Scene::GameLevel::Create2(engine.keyEventData);
      Temp::Scene::SceneFns mainMenuLevel = Scene::MainMenuLevel::Create();
      mainMenuLevel.nextScene = &scene1;
      scene1.nextScene = &scene2;
      scene2.nextScene = &scene1;
      engine.sceneFns.push_back(mainMenuLevel);
      engine.sceneFns.push_back(scene1);
      engine.sceneFns.push_back(scene2);
      engine.coordinatorFns.Init = Game::Coordinator::Init;
      engine.coordinatorFns.Destruct = Game::Coordinator::Destruct;
      engine.coordinatorFns.Reset = Game::Coordinator::Reset;

      // Testing Add and Remove works
      Input::AddCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      Input::RemoveCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      Input::AddCallback(Exit, engine.keyEventData, Temp::Input::KeyboardCode::KB_Q);
      
//      lua_State *L;
//      L = luaL_newstate();
//      luaL_openlibs(L);
//
//      auto begin = clock();
//
//      while(true)
//      {
//        if (((float)(clock() - begin) / CLOCKS_PER_SEC) < 1)
//        {
//          continue;
//        }
//        if (luaL_loadfile(L, "LuaScripts/Test.lua")) {
//          cerr << "Something went wrong loading the chunk (syntax error?)" << endl;
//          cerr << lua_tostring(L, -1) << endl;
//          lua_pop(L,1);
//        }
//
//        lua_pushnumber(L, 1.1);
//        lua_setglobal(L, "cppvar");
//
//        if (lua_pcall(L,0, LUA_MULTRET, 0)) {
//          cerr << "Something went wrong during execution" << endl;
//          cerr << lua_tostring(L, -1) << endl;
//          lua_pop(L,1);
//        }
//
//        lua_getglobal(L, "luavar");
//        //        double luavar = lua_tonumber(L,-1);
//        lua_pop(L,1);
//
//        lua_getglobal(L, "myluafunction");
//        lua_pushnumber(L, 5);
//        lua_pcall(L, 1, 1, 0);
//        lua_pop(L,1);
//
//        //        lua_pushcfunction(L,l_cppfunction);
//        //        lua_setglobal(L, "cppfunction");
//        //
//        //        lua_getglobal(L, "myfunction");
//        //        lua_pushnumber(L, 5);
//        //        lua_pcall(L, 1, 1, 0);
//        //        lua_pop(L,1);
//
//        begin = clock();
//      }
//
//      cout << "** Release the Lua enviroment" << endl;
//      lua_close(L);
      
      Temp::Engine::Run(engine, windowName, 1280, 720);
    }
  }
}
