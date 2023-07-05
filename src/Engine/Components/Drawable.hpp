#pragma once

#include "Math.hpp"
#include "OpenGLWrapper.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include <vector>

// TODO: Clean up resources, VAO, VBO, EBO, texture, shaderProgram etc.
namespace Temp::Component::Drawable
{
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
#ifdef DEBUG
    std::string shaderPath{};
    std::filesystem::file_time_type time{};
    int shaderIdx{};
    int bufferDraw{};
    int vertexStride{};
    GLuint UBO{};
    std::string UBOMatrices{};
    int UBOMatrixIndex{};
#endif
    bool operator==(const Data& other) const = default;
  };

  constexpr void Scale(Data &drawable, const Math::Vec3f& scale)
  {
    drawable.model = drawable.model.scale(scale);
  }

  inline void Update(Data &drawable)
  {
    using namespace Temp::Render;

    OpenGLWrapper::Set4x4MatrixShaderProperty(drawable.shaderProgram, "model", &drawable.model.rows[0][0]);
  }

  inline void Construct(Data &drawable, int shaderIdx, int bufferDraw, int vertexStride, int UBO, const char * UBOMatrices, int UBOMatricesIdx)
  {
    using namespace Temp::Render;
    
    drawable.vertices.shrink_to_fit();
    drawable.indices.shrink_to_fit();

    drawable.shaderProgram = OpenGLWrapper::CreateShaderProgram(shaderIdx);
    drawable.VAO = OpenGLWrapper::CreateVAO();
    drawable.VBO = OpenGLWrapper::CreateVBO(drawable.vertices.data(), drawable.vertices.size(), bufferDraw);
    drawable.EBO = OpenGLWrapper::CreateEBO(drawable.indices.data(), drawable.indices.size(), bufferDraw);
    drawable.indicesSize = (int)drawable.indices.size();
    OpenGLWrapper::SetVertexAttribArray(0, vertexStride, vertexStride, 0);

    Update(drawable);

    // We are making the assumption that every shader has a Matrices uniform block
    // For getting view and projection from the camera
    OpenGLWrapper::BindUBOShader(UBO, drawable.shaderProgram, UBOMatrices, UBOMatricesIdx);
  }

  // Make sure all API construction happens before render-thread executes!
  inline void Construct(Data &drawable, int shaderIdx, int bufferDraw = GL_STATIC_DRAW, int vertexStride = 3, int UBO = Camera::UBO())
  {
    using namespace Temp::Render;

#ifdef DEBUG
    drawable.shaderIdx = shaderIdx;
    drawable.shaderPath = OpenGLWrapper::GetShadersPath() / OpenGLWrapper::ShaderFiles()[shaderIdx];
    drawable.UBOMatrices = "Matrices";
    drawable.UBOMatrixIndex = 0;
#endif
    Construct(drawable, shaderIdx, bufferDraw, vertexStride, UBO, "Matrices", 0);
  }

  inline void ConstructFont(Data &drawable, int shaderIdx, int bufferDraw = GL_DYNAMIC_DRAW, int vertexStride = 4, int UBO = Camera::FontUBO())
  {
    using namespace Temp::Render;

#ifdef DEBUG
    drawable.shaderIdx = shaderIdx;
    drawable.shaderPath = OpenGLWrapper::GetShadersPath() / OpenGLWrapper::ShaderFiles()[shaderIdx];
    drawable.UBOMatrices = "FontMatrices";
    drawable.UBOMatrixIndex = 1;
#endif
    Construct(drawable, shaderIdx, bufferDraw, vertexStride, UBO, "FontMatrices", 1);
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
    OpenGLWrapper::DrawElementsInstanced(drawable.VAO, drawable.indicesSize, drawable.numInstances);
    glDepthMask(GL_TRUE);
  }

  inline void UpdateData(Data& drawable, const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
  {
    drawable.vertices.assign(vertices.begin(), vertices.end());
    drawable.indices.assign(indices.begin(), indices.end());
    drawable.indicesSize = (int)indices.size();
  }
  
  inline void Destruct(Data& drawable)
  {
    using namespace Temp::Render::OpenGLWrapper;
    
    CleanArrays(drawable.VAO);
    CleanBuffer(drawable.VBO);
    CleanBuffer(drawable.EBO);
    CleanShader(drawable.shaderProgram);
  }
}
