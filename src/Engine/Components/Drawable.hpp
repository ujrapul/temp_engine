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
      // Represents the viewpoint of the camera
      static Math::Mat4 view = Math::Mat4().translate(Math::Vec3f(0, 0, 0));
      static float scale = 8.0f;
      static float aspect = static_cast<float>(1024) / static_cast<float>(768);
      static Math::Mat4 projection = Math::Mat4::orthographic(-aspect * scale, aspect * scale, -scale, scale, -100, 100);
      // static Math::Mat4 projection = Math::Mat4::perspective(Math::ToRadians(45.f), 1.33333333333, 0.1, 100);

      struct Data
      {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        Math::Mat4 model{};
        GLuint VAO{};
        GLuint VBO{};
        GLuint EBO{};
        GLuint texture{};
        GLuint shaderProgram{};
        int numInstances{1};
      };

      // Make sure all construction happens before render-thread executes!
      inline void Construct(Data *data, int shaderIdx)
      {
        using namespace Temp::Render;

        data->shaderProgram = OpenGLWrapper::CreateShaderProgram(shaderIdx);

        // Create vertex array object (VAO)
        data->VAO = OpenGLWrapper::CreateVAO();

        // Create vertex buffer object (VBO)
        data->VBO = OpenGLWrapper::CreateVBO(data->vertices.data(), data->vertices.size());

        // Create Element Buffer Object (EBO) and copy index data
        data->EBO = OpenGLWrapper::CreateEBO(data->indices.data(), data->indices.size());

        // Unbind VAO and VBO and EBO
        // OpenGLWrapper::UnbindBuffers();
      }

      inline void Draw(Data *data)
      {
        using namespace Temp::Render;

        // std::cout << "GOT TO DRAW!" << std::endl;

        // data->model = data->model.rotate(0.01f * Math::ToRadians(50.0f), Math::Vec3f(0.5, 1.0, 0));

        OpenGLWrapper::Set4x4MatrixShaderProperty(data->shaderProgram, "model", &data->model.rows[0][0]);
        OpenGLWrapper::Set4x4MatrixShaderProperty(data->shaderProgram, "view", &view.rows[0][0]);
        OpenGLWrapper::Set4x4MatrixShaderProperty(data->shaderProgram, "projection", &projection.rows[0][0]);

        OpenGLWrapper::BindTexture(GL_TEXTURE0, data->texture);

        // std::cout << data->indices.size() << " " << data->vertices.size() << std::endl;

        // Bind the VAO and draw the triangle
        OpenGLWrapper::DrawElementsInstanced(data->VAO, data->indices.size(), data->numInstances);
      }
    }
  }
}