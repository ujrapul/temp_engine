#pragma once

#include "Math.hpp"
#include "OpenGLWrapper.hpp"
#include "Camera.hpp"
#include <vector>

// TODO: Clean up resources, VAO, VBO, EBO, texture, shaderProgram etc.
namespace Temp::Component::Drawable
{
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
  void Construct(Data *data, int shaderIdx, int BufferDraw = GL_STATIC_DRAW, int vertexStride = 3, int UBO = Camera::UBO());
  void ConstructFont(Data *data, int shaderIdx, int BufferDraw = GL_DYNAMIC_DRAW, int vertexStride = 4, int UBO = Camera::FontUBO());
  void Update(Data *data);
  void Draw(Data *data);
}