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
  void UpdateFontOrthoScale(Scene::Data *scene, float _fontOrthoScale);
  void UpdateCameraAspect(Scene::Data *scene, float _width, float _height);
  void UpdateCameraAspect(Engine::Data &engine, float _width, float _height);
  float GetAspect();
  float GetHeight();
  void UpdateFov(Scene::Data *scene, float _fov);
  void SetProjection(Projection _projection);
  void TranslateView(const Math::Vec3f &translate);
  void ResetView();
  const Math::Mat4& GetOrthoProjecton();
}