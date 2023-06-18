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
    bool visible{true};
  };

  constexpr void Update(Data *data)
  {
    using namespace Temp::Render;

    OpenGLWrapper::Set4x4MatrixShaderProperty(data->shaderProgram, "model", &data->model.rows[0][0]);
  }

  // Make sure all API construction happens before render-thread executes!
  constexpr void Construct(Data *data, int shaderIdx, int BufferDraw = GL_STATIC_DRAW, int vertexStride = 3, int UBO = Camera::UBO())
  {
    using namespace Temp::Render;

    data->shaderProgram = OpenGLWrapper::CreateShaderProgram(shaderIdx);

    // Create vertex array object (VAO)
    data->VAO = OpenGLWrapper::CreateVAO();

    // Create vertex buffer object (VBO)
    data->VBO = OpenGLWrapper::CreateVBO(data->vertices.data(), data->vertices.size(), BufferDraw);

    // Create Element Buffer Object (EBO) and copy index data
    data->EBO = OpenGLWrapper::CreateEBO(data->indices.data(), data->indices.size(), BufferDraw);

    // Specify vertex attributes
    // position attribute
    OpenGLWrapper::SetVertexAttribArray(0, vertexStride, vertexStride, 0);

    Update(data);

    // We are making the assumption that every shader has a Matrices uniform block
    // For getting view and projection from the camera
    OpenGLWrapper::BindUBOShader(UBO, data->shaderProgram, "Matrices", 0);

    // Unbind VAO and VBO and EBO
    // OpenGLWrapper::UnbindBuffers();
  }
  
  constexpr void ConstructFont(Data *data, int shaderIdx, int BufferDraw = GL_DYNAMIC_DRAW, int vertexStride = 4, int UBO = Camera::FontUBO())
  {
    using namespace Temp::Render;

    data->shaderProgram = OpenGLWrapper::CreateShaderProgram(shaderIdx);

    data->VAO = OpenGLWrapper::CreateVAO();
    data->VBO = OpenGLWrapper::CreateVBO(data->vertices.data(), data->vertices.size(), BufferDraw);
    data->EBO = OpenGLWrapper::CreateEBO(data->indices.data(), data->indices.size(), BufferDraw);

    OpenGLWrapper::SetVertexAttribArray(0, vertexStride, vertexStride, 0);

    Update(data);

    OpenGLWrapper::BindUBOShader(UBO, data->shaderProgram, "FontMatrices", 1);

    // Unbind VAO and VBO and EBO
    // OpenGLWrapper::UnbindBuffers();
  }

  constexpr void Draw(Data *data)
  {
    using namespace Temp::Render;

    if (!data->visible)
    {
      return;
    }

    glUseProgram(data->shaderProgram);
    OpenGLWrapper::BindTexture(GL_TEXTURE0, data->texture);

    // Bind the VAO and draw the triangle
    OpenGLWrapper::DrawElementsInstanced(data->VAO, data->indices.size(), data->numInstances);
  }
}