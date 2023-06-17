#include "Camera.hpp"
#include "Scene.hpp"

namespace Temp::Camera
{
  namespace
  {
    float orthoScale = 1.f;
    float aspect = 1024.f / 768.f;
    float fov = 45.f;

    Projection projection = Projection::PERSPECTIVE;

    Math::Mat4 orthoProjection = Math::Mat4::orthographic(-aspect * orthoScale,
                                                          aspect *orthoScale,
                                                          -orthoScale, orthoScale,
                                                          -100, 100);

    Math::Mat4 perspProjection = Math::Mat4::perspective(Math::ToRadians(fov), 1.33333333333, 0.1, 100);

    Math::Mat4 view{};

    Math::Mat4 &GetProjection()
    {
      switch (projection)
      {
      case Projection::PERSPECTIVE:
        return perspProjection;
      default:
        return orthoProjection;
      };
    }

    const Math::Mat4 &GetProjection(Projection _projection)
    {
      switch (_projection)
      {
      case Projection::PERSPECTIVE:
        return perspProjection;
      default:
        return orthoProjection;
      };
    }

    void UpdateCamera(Scene::Data *scene, void * /*data*/)
    {
      using namespace Temp::Render;

      // std::cout << sizeof(Math::Mat4) << std::endl;

      // Matrices need to be transposed since OpenGL uses column major matrices
      OpenGLWrapper::UpdateUBO(UBO(), &GetProjection().transpose().rows[0][0], sizeof(Math::Mat4), 0);
      OpenGLWrapper::UpdateUBO(UBO(), &view.transpose().rows[0][0], sizeof(Math::Mat4), sizeof(Math::Mat4));
    }
  }

  GLuint UBO()
  {
    // 128 bytes because this UBO contains the View and Projection matrices
    static GLuint UBO = Render::OpenGLWrapper::CreateUBO(2 * sizeof(Math::Mat4));
    return UBO;
  }

  void UpdateOrthoScale(Scene::Data *scene, float _orthoScale)
  {
    orthoScale = _orthoScale;
    orthoProjection = Math::Mat4::orthographic(-aspect * orthoScale,
                                               aspect * orthoScale,
                                               -orthoScale, orthoScale,
                                               -100, 100);
    Temp::Scene::EnqueueRender(scene, UpdateCamera, nullptr);
  }

  void UpdateCameraAspect(Scene::Data *scene, float _aspect)
  {
    aspect = _aspect;
    orthoProjection = Math::Mat4::orthographic(-aspect * orthoScale,
                                               aspect * orthoScale,
                                               -orthoScale, orthoScale,
                                               -100, 100);
    Temp::Scene::EnqueueRender(scene, UpdateCamera, nullptr);
  }

  void UpdateFov(Scene::Data *scene, float _fov)
  {
    fov = _fov;
    perspProjection = Math::Mat4::perspective(Math::ToRadians(fov), 1.33333333333, 0.1, 100);
    Temp::Scene::EnqueueRender(scene, UpdateCamera, nullptr);
  }

  // Should only be called once!
  void SetProjection(Projection _projection)
  {
    projection = _projection;
  }

  void TranslateView(const Math::Vec3f &translate)
  {
    view = view.translate(translate);
  }

  const Math::Mat4& GetOrthoProjecton()
  {
    return GetProjection(Projection::ORTHOGRAPHIC);
  }
}