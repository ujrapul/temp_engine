#pragma once

#include "Math.hpp"
#include "OpenGLWrapper.hpp"
#include <vector>

// TODO: Clean up resources, VAO, VBO, EBO, texture, shaderProgram etc.

namespace Temp
{
  namespace Component
  {
    namespace Drawable
    {
      enum class Projection
      {
        PERSPECTIVE,
        ORTHOGRAPHIC
      };

      struct Camera
      {
        float orthoScale{32.0f};
        float aspect{1024.f / 768.f};
        float fov{45.f};
        Math::Mat4 view{Math::Mat4()};
      };

      Camera *GetCamera();
      void UpdateOrthoScale(float orthoScale);
      void UpdateCameraAspect(float aspect);
      void UpdateFov(float fov);
      void SetProjection(Projection _projection);

      struct Data
      {
        std::vector<float> vertices{};
        std::vector<unsigned int> indices{};
        Math::Mat4 model{};
        GLuint VAO{};
        GLuint VBO{};
        GLuint EBO{};
        GLuint texture{};
        GLuint shaderProgram{};
        int numInstances{1};
      };

      // Make sure all API construction happens before render-thread executes!
      void Construct(Data *data, int shaderIdx);
      void Draw(Data *data);
    }
  }
}