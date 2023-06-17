#pragma once

#include "Math.hpp"
#include "gl.h"

namespace Temp::Scene
{
  struct Data;
}

namespace Temp::Engine
{
  struct Data;
}

namespace Temp::Camera
{
  enum class Projection
  {
    PERSPECTIVE,
    ORTHOGRAPHIC
  };

  GLuint UBO();
  GLuint FontUBO();
  void UpdateOrthoScale(Scene::Data *scene, float _orthoScale);
  void UpdateCameraAspect(Scene::Data *scene, float _aspect);
  void UpdateCameraAspect(Engine::Data &engine, float _aspect);
  float GetAspect();
  void UpdateFov(Scene::Data *scene, float _fov);
  void SetProjection(Projection _projection);
  void TranslateView(const Math::Vec3f &translate);
  const Math::Mat4& GetOrthoProjecton();
}