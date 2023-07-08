#include "SceneObject.hpp"
#include "TextBox.hpp"
#include "TextButton.hpp"

namespace Temp::SceneObject
{
  namespace
  {
    struct Null
    {
    };

    template <Type::Type type, typename T, typename C = Null>
    void Init()
    {
      if constexpr (std::is_same<C, Null>::value)
      {
        FnTable[FnType::CONSTRUCT][type] = [](auto &scene, auto &object)
        { Construct(scene, *static_cast<T *>(object.data)); };
      }
      else
      {
        FnTable[FnType::CONSTRUCT][type] = [](auto &scene, auto &object)
        { Construct(scene, *static_cast<T *>(object.data), *static_cast<C *>(object.constructData)); };
      }
      FnTable[FnType::DESTRUCT][type] = [](auto &, auto &object)
      {
        Destruct(*static_cast<T *>(object.data));
        delete static_cast<T *>(object.data);
        delete static_cast<C *>(object.constructData);
      };
      FnTable[FnType::DRAWCONSTRUCT][type] = [](auto &scene, auto &object)
      { DrawConstruct(scene, *static_cast<T *>(object.data)); };
      FnTable[FnType::DRAWDESTRUCT][type] = [](auto &scene, auto &object)
      { DrawDestruct(scene, *static_cast<T *>(object.data)); };
      FnTable[FnType::DRAWUPDATE][type] = [](auto &scene, auto &object)
      { UpdateRender(scene, *static_cast<T *>(object.data)); };
      DrawReloadTable[type] = [](auto &scene, auto &object, auto shaderIdx)
      { DrawReload(scene, *static_cast<T *>(object.data), shaderIdx); };
      UpdateTable[type] = [](auto &scene, auto &object, auto deltaTime)
      { Update(scene, *static_cast<T *>(object.data), deltaTime); };
    }
  }

  void Init()
  {
    Init<Type::TEXTBOX, TextBox::Data>();
    Init<Type::TEXTBUTTON, TextButton::Data, Component::Hoverable::Data>();
  }
}
