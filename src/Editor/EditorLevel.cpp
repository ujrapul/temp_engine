#include "EditorLevel.hpp"

namespace Temp::Scene::EditorLevel
{
  namespace
  {
    void Construct(Scene::Data& /*unused*/)
    {
    }

    void Update(Scene::Data& /*unused*/, float /*unused*/)
    {
    }

    void DrawConstruct(Scene::Data& /*unused*/)
    {
    }

    void DrawUpdate(Scene::Data& scene)
    {
      Temp::Camera::UpdateFontOrthoScale(scene, (1080.f / Temp::Camera::GetHeight()));
    }

    void Destruct(Scene::Data& /*unused*/)
    {
    }

    void DrawDestruct(Scene::Data& /*unused*/)
    {

    }

    void DrawReload(Scene::Data& /*unused*/, int /*unused*/)
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