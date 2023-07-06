#pragma once

namespace Temp::Scene
{
  struct Data;
}

namespace Temp::SceneObject
{
  namespace Type
  {
    enum Type
    {
      TEXTBOX = 0,
      TEXTBUTTON,
      MAX
    };
  }
  
  namespace FnType
  {
    enum FnType
    {
      CONSTRUCT = 0,
      DESTRUCT,
      DRAWCONSTRUCT,
      DRAWDESTRUCT,
      DRAWUPDATE,
      MAX
    };
  }
  
  struct Data
  {
    void* data;
    Type::Type type;
  };
  
  inline void(*FnTable[FnType::MAX][256])(Scene::Data&, Data&);
  inline void(*DrawReloadTable[256])(Scene::Data&, Data&, int);
  inline void(*UpdateTable[256])(Scene::Data&, Data&, float);
  void Init();
  
  constexpr void Construct(Scene::Data& scene, Data& object)
  {
    FnTable[FnType::CONSTRUCT][object.type](scene, object);
  }
};
