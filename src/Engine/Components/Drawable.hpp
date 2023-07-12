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
    std::vector<GLuint> buffers{};
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
    bool updateModel{false};
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

    // Needed for unit test
    bool operator==(const Data &other) const = default;
  };

  inline void Update(Data &drawable)
  {
    using namespace Temp::Render;

    OpenGLWrapper::Set4x4MatrixShaderProperty(drawable.shaderProgram, "model", &drawable.model.rows[0][0]);
  }

  constexpr void Scale(Data &drawable, const Math::Vec3f &scale)
  {
    drawable.model = drawable.model.scale(scale);
    drawable.updateModel = true;
  }

  constexpr void Translate(Data &drawable, const Math::Vec3f &translate)
  {
    drawable.model = drawable.model.translate(translate);
    drawable.updateModel = true;
  }

  constexpr void SetTranslate(Data &drawable, const Math::Vec3f &translate)
  {
    drawable.model.setTranslation(translate);
    drawable.updateModel = true;
  }

  inline void Construct(Data &drawable, int shaderIdx, int bufferDraw, int vertexStride, int UBO, const char *UBOMatrices, int UBOMatricesIdx)
  {
    using namespace Temp::Render;

    drawable.shaderProgram = OpenGLWrapper::CreateShaderProgram(shaderIdx);
    drawable.VAO = OpenGLWrapper::CreateVAO();
    drawable.VBO = OpenGLWrapper::CreateVBO(drawable.vertices.data(), drawable.vertices.size(), bufferDraw);
    drawable.EBO = OpenGLWrapper::CreateEBO(drawable.indices.data(), drawable.indices.size(), bufferDraw);
    drawable.indicesSize = (int)drawable.indices.size();
    OpenGLWrapper::SetVertexAttribArray(0, vertexStride, vertexStride, 0);

    FreeContainer(drawable.vertices);
    FreeContainer(drawable.indices);

    Update(drawable);

    // We are making the assumption that every shader has a Matrices uniform block
    // For getting view and projection from the camera
    OpenGLWrapper::BindUBOShader(UBO, drawable.shaderProgram, UBOMatrices, UBOMatricesIdx);
    // OpenGLWrapper::UnbindBuffers();
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

    if (drawable.updateModel)
    {
      Update(drawable);
      drawable.updateModel = false;
    }

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

  inline void UpdateData(Data &drawable, std::vector<float> vertices, std::vector<unsigned int> indices)
  {
    drawable.vertices = std::move(vertices);
    drawable.indices = std::move(indices);
    drawable.indicesSize = (int)drawable.indices.size();
  }

  inline void Destruct(Data &drawable)
  {
    using namespace Temp::Render::OpenGLWrapper;

    // IMPORTANT: Make sure to clean up buffers so that vector data can be released and destructed
    // Otherwise memory usage will slowly climb on every new instance of this object
    for (auto buffer : drawable.buffers)
    {
      CleanArrayBuffer(buffer);
    }
    CleanElementBuffer(drawable.EBO);
    CleanArrayBuffer(drawable.VBO);
    CleanArrays(drawable.VAO);
    CleanShader(drawable.shaderProgram);
    UnbindBuffers();

    FreeContainer(drawable.buffers);
    FreeContainer(drawable.vertices);
    FreeContainer(drawable.indices);
  }

  template <typename T>
  inline void CreateBuffer(Data &drawable, std::vector<T> &data)
  {
    using namespace Temp::Render::OpenGLWrapper;
    drawable.buffers.push_back(CreateVBO(data.data(), sizeof(T), data.size(), GL_STATIC_DRAW));
  }

  inline void UpdateFloatBuffer(GLuint buffer, std::vector<float> data, int BufferDraw = GL_STATIC_DRAW)
  {
    using namespace Temp::Render::OpenGLWrapper;
    UpdateVBO(buffer, data.data(), data.size(), BufferDraw);
    FreeContainer(data);
  }

  inline void UpdateIndexBuffer(GLuint buffer, std::vector<unsigned int> data, int BufferDraw = GL_STATIC_DRAW)
  {
    using namespace Temp::Render::OpenGLWrapper;
    UpdateEBO(buffer, data.data(), data.size(), BufferDraw);
    FreeContainer(data);
  }

  inline GLuint CreateFloatBuffer(Data &drawable, std::vector<float> &data, int arrayIndex, int numOfElements, int stride, int position = 0)
  {
    using namespace Temp::Render::OpenGLWrapper;
    CreateBuffer(drawable, data);
    SetVertexAttribArray(arrayIndex, numOfElements, stride, position);
    return drawable.buffers.back();
  }

  inline GLuint CreateIntBuffer(Data &drawable, std::vector<int> &data, int arrayIndex, int numOfElements, int stride, int position = 0)
  {
    using namespace Temp::Render::OpenGLWrapper;
    CreateBuffer(drawable, data);
    SetVertexIAttribArray(arrayIndex, numOfElements, stride, position);
    return drawable.buffers.back();
  }

  inline GLuint CreateFloatInstancedBuffer(Data &drawable, std::vector<float> &data, int arrayIndex, int numOfElements, int stride, int position = 0)
  {
    using namespace Temp::Render::OpenGLWrapper;
    CreateBuffer(drawable, data);
    SetVertexAttribArrayInstanced(arrayIndex, numOfElements, stride, position);
    return drawable.buffers.back();
  }

  inline GLuint CreateIntInstancedBuffer(Data &drawable, std::vector<int> &data, int arrayIndex, int numOfElements, int stride, int position = 0)
  {
    using namespace Temp::Render::OpenGLWrapper;
    CreateBuffer(drawable, data);
    SetVertexAttribIArrayInstanced(arrayIndex, numOfElements, stride, position);
    return drawable.buffers.back();
  }

  inline void UpdateVertexIndexBuffers(Data &drawable, int BufferDraw = GL_STATIC_DRAW)
  {
    UpdateFloatBuffer(drawable.VBO, std::move(drawable.vertices), BufferDraw);
    UpdateIndexBuffer(drawable.EBO, std::move(drawable.indices), BufferDraw);
  }
}
