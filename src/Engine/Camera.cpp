#include "Camera.hpp"
#include "Scene.hpp"
#include "Engine.hpp"

namespace Temp::Camera
{
  namespace
  {
    float orthoScale{1.f};
    float fontOrthoScale{1.f};
    float aspect{1024.f / 768.f};
    float width{1280};
    float height{720};
    float fov{45.f};

    Math::Mat4 FontOrthoProjection()
    {
      aspect = width / height;
      return Math::Mat4::orthographic(-width / 2 * fontOrthoScale, width / 2 * fontOrthoScale,
                                      -height / 2 * fontOrthoScale, height / 2 * fontOrthoScale,
                                      -100, 100);
    }

    Math::Mat4 OrthoProjection()
    {
      aspect = width / height;
      return Math::Mat4::orthographic(-width / 2 * orthoScale, width / 2 * orthoScale,
                                      -height / 2 * orthoScale, height / 2 * orthoScale,
                                      -100, 100);
    }

    Projection projection = Projection::PERSPECTIVE;

    Math::Mat4 orthoProjection = OrthoProjection();
    Math::Mat4 fontOrthoProjection = FontOrthoProjection();

    Math::Mat4 perspProjection = Math::Mat4::perspective(Math::ToRadians(fov), aspect, 0.1, 100);

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

    void UpdateCamera(Scene::Data & /*scene*/, void * /*data*/)
    {
      using namespace Temp::Render;

      // std::cout << sizeof(Math::Mat4) << std::endl;

      // Matrices need to be transposed since OpenGL uses column major matrices
      OpenGLWrapper::UpdateUBO(UBO(), &GetProjection().transpose().rows[0][0], sizeof(Math::Mat4), 0);
      OpenGLWrapper::UpdateUBO(UBO(), &view.transpose().rows[0][0], sizeof(Math::Mat4), sizeof(Math::Mat4));
      OpenGLWrapper::UpdateUBO(FontUBO(), &fontOrthoProjection.transpose().rows[0][0], sizeof(Math::Mat4), 0);
    }

    void UpdateCameraEngine(void * /*data*/)
    {
      using namespace Temp::Render;

      // std::cout << sizeof(Math::Mat4) << std::endl;

      // Matrices need to be transposed since OpenGL uses column major matrices
      OpenGLWrapper::UpdateUBO(UBO(), &GetProjection().transpose().rows[0][0], sizeof(Math::Mat4), 0);
      OpenGLWrapper::UpdateUBO(UBO(), &view.transpose().rows[0][0], sizeof(Math::Mat4), sizeof(Math::Mat4));
      OpenGLWrapper::UpdateUBO(FontUBO(), &fontOrthoProjection.transpose().rows[0][0], sizeof(Math::Mat4), 0);
    }
  }

  GLuint UBO()
  {
    // 128 bytes because this UBO contains the View and Projection matrices
    static GLuint UBO = Render::OpenGLWrapper::CreateUBO(2 * sizeof(Math::Mat4));
    return UBO;
  }

  GLuint FontUBO()
  {
    // 128 bytes because this UBO contains the View and Projection matrices
    static GLuint FontUBO = Render::OpenGLWrapper::CreateUBO(sizeof(Math::Mat4));
    return FontUBO;
  }

  void UpdateOrthoScale(Scene::Data &scene, float _orthoScale)
  {
    orthoScale = _orthoScale;
    orthoProjection = std::move(OrthoProjection());
    Temp::Scene::EnqueueRender(scene, UpdateCamera, nullptr);
  }

  void UpdateFontOrthoScale(Scene::Data &scene, float _orthoScale)
  {
    fontOrthoScale = _orthoScale;
    fontOrthoProjection = std::move(FontOrthoProjection());
    Temp::Scene::EnqueueRender(scene, UpdateCamera, nullptr);
  }

  void UpdateCameraAspect(Scene::Data &scene, float _width, float _height)
  {
    width = _width;
    height = _height;
    aspect = width / height;
    orthoProjection = std::move(OrthoProjection());
    fontOrthoProjection = std::move(FontOrthoProjection());
    perspProjection = std::move(Math::Mat4::perspective(Math::ToRadians(fov), aspect, 0.1, 100));
    Temp::Scene::EnqueueRender(scene, UpdateCamera, nullptr);
  }

  void UpdateCameraAspect(Engine::Data &engine, float _width, float _height)
  {
    width = _width;
    height = _height;
    aspect = width / height;
    orthoProjection = std::move(OrthoProjection());
    fontOrthoProjection = std::move(FontOrthoProjection());
    perspProjection = std::move(Math::Mat4::perspective(Math::ToRadians(fov), aspect, 0.1, 100));
    Temp::Engine::EnqueueGlobalRender(engine, UpdateCameraEngine, nullptr);
  }

  float GetAspect()
  {
    return aspect;
  }

  float GetHeight()
  {
    return height;
  }

  void UpdateFov(Scene::Data &scene, float _fov)
  {
    fov = _fov;
    perspProjection = Math::Mat4::perspective(Math::ToRadians(fov), aspect, 0.1, 100);
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

  void ResetView()
  {
    view = {};
  }

  const Math::Mat4 &GetOrthoProjecton()
  {
    return GetProjection(Projection::ORTHOGRAPHIC);
  }

  Math::Vec2f ConvertToFontOrthoViewSpace(float x, float y)
  {
    float normalizedX = x / width;
    float normalizedY = y / height;

    return {(-width / 2 + width * normalizedX) * fontOrthoScale,
            (height / 2 - height * normalizedY) * fontOrthoScale};
  }
}
