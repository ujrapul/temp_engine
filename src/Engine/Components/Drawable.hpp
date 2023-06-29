#pragma once

#include "Math.hpp"
#include "OpenGLWrapper.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include <vector>

// TODO: Clean up resources, VAO, VBO, EBO, texture, shaderProgram etc.
namespace Temp::Component::Drawable
{
  inline std::mutex mtx{};
  inline std::condition_variable cv{};

  struct Data
  {
    std::vector<float> vertices{};
    std::vector<unsigned int> indices{};
    Math::Mat4 model{};
    Entity entity{UINT32_MAX};
    GLuint VAO{};
    GLuint VBO{};
    GLuint EBO{};
    GLuint texture{};
    GLuint shaderProgram{};
    int numInstances{1};
    int indicesSize{0};
    bool visible{true};
    bool blockDraw{false};
    bool disableDepth{false};
  };

  constexpr void Scale(Data &drawable, const Math::Vec3f scale)
  {
    drawable.model = drawable.model.scale(scale);
  }

  inline void Update(Data &drawable)
  {
    using namespace Temp::Render;

    OpenGLWrapper::Set4x4MatrixShaderProperty(drawable.shaderProgram, "model", &drawable.model.rows[0][0]);
  }

  // Make sure all API construction happens before render-thread executes!
  inline void Construct(Data &drawable, int shaderIdx, int BufferDraw = GL_STATIC_DRAW, int vertexStride = 3, int UBO = Camera::UBO())
  {
    using namespace Temp::Render;

    drawable.shaderProgram = OpenGLWrapper::CreateShaderProgram(shaderIdx);

    // Create vertex array object (VAO)
    drawable.VAO = OpenGLWrapper::CreateVAO();

    // Create vertex buffer object (VBO)
    drawable.VBO = OpenGLWrapper::CreateVBO(drawable.vertices.data(), drawable.vertices.size(), BufferDraw);

    // Create Element Buffer Object (EBO) and copy index data
    drawable.EBO = OpenGLWrapper::CreateEBO(drawable.indices.data(), drawable.indices.size(), BufferDraw);
    
    drawable.indicesSize = (int)drawable.indices.size();

    // Specify vertex attributes
    // position attribute
    OpenGLWrapper::SetVertexAttribArray(0, vertexStride, vertexStride, 0);

    Update(drawable);

    // We are making the assumption that every shader has a Matrices uniform block
    // For getting view and projection from the camera
    OpenGLWrapper::BindUBOShader(UBO, drawable.shaderProgram, "Matrices", 0);

    // Unbind VAO and VBO and EBO
    // OpenGLWrapper::UnbindBuffers();
  }

  inline void ConstructFont(Data &drawable, int shaderIdx, int BufferDraw = GL_DYNAMIC_DRAW, int vertexStride = 4, int UBO = Camera::FontUBO())
  {
    using namespace Temp::Render;

    drawable.shaderProgram = OpenGLWrapper::CreateShaderProgram(shaderIdx);

    drawable.VAO = OpenGLWrapper::CreateVAO();
    drawable.VBO = OpenGLWrapper::CreateVBO(drawable.vertices.data(), drawable.vertices.size(), BufferDraw);
    drawable.EBO = OpenGLWrapper::CreateEBO(drawable.indices.data(), drawable.indices.size(), BufferDraw);
    drawable.indicesSize = (int)drawable.indices.size();

    OpenGLWrapper::SetVertexAttribArray(0, vertexStride, vertexStride, 0);

    Update(drawable);

    OpenGLWrapper::BindUBOShader(UBO, drawable.shaderProgram, "FontMatrices", 1);

    // Unbind VAO and VBO and EBO
    // OpenGLWrapper::UnbindBuffers();
  }

  inline void Draw(Data &drawable)
  {
    using namespace Temp::Render;
    
    if (!drawable.visible || drawable.blockDraw)
    {
      return;
    }


    glDepthMask(!drawable.disableDepth); // Don't write into the depth buffer
    glUseProgram(drawable.shaderProgram);
    OpenGLWrapper::BindTexture(GL_TEXTURE0, drawable.texture);

    // Bind the VAO and draw the triangle
    OpenGLWrapper::DrawElementsInstanced(drawable.VAO, (int)drawable.indices.size(), drawable.numInstances);
    glDepthMask(GL_TRUE);
  }
}
