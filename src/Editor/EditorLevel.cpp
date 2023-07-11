#include "EditorLevel.hpp"

namespace Temp::Scene::EditorLevel
{
  namespace
  {
    void Construct(Scene::Data&)
    {
    }

    void Update(Scene::Data&, float)
    {
    }

    void DrawConstruct(Scene::Data&)
    {
    }

    void DrawUpdate(Scene::Data&)
    {
    }

    void Destruct(Scene::Data&)
    {
    }

    void DrawDestruct(Scene::Data&)
    {

    }

    void DrawReload(Scene::Data&, int)
    {
    }
  }

  SceneFns Create()
  {
    SceneFns scene{};
    scene.ConstructFunc = Construct;
    scene.Update = Update;
    scene.DrawConstructFunc = DrawConstruct;
    scene.DrawUpdateFunc = DrawUpdate;
    scene.DestructFunc = Destruct;
    scene.DrawDestructFunc = DrawDestruct;
    scene.DrawReloadFunc = DrawReload;
    return scene;
  }
}