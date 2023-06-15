#include "Drawable.hpp"

namespace Temp
{
  namespace Component
  {
    namespace Drawable
    {
      namespace
      {
        Projection projection = Projection::PERSPECTIVE;

        Math::Mat4 orthoProjection = Math::Mat4::orthographic(-GetCamera()->aspect * GetCamera()->orthoScale,
                                                              GetCamera()->aspect *GetCamera()->orthoScale,
                                                              -GetCamera()->orthoScale, GetCamera()->orthoScale,
                                                              -100, 100);

        Math::Mat4 perspProjection = Math::Mat4::perspective(Math::ToRadians(GetCamera()->fov), 1.33333333333, 0.1, 100);

        Math::Mat4 GetProjection()
        {
          switch (projection)
          {
          case Projection::PERSPECTIVE:
            return perspProjection;
          default:
            return orthoProjection;
          };
        }
      }

      void UpdateOrthoScale(float orthoScale)
      {
        orthoProjection = Math::Mat4::orthographic(-GetCamera()->aspect * GetCamera()->orthoScale,
                                                   GetCamera()->aspect * GetCamera()->orthoScale,
                                                   -GetCamera()->orthoScale, GetCamera()->orthoScale,
                                                   -100, 100);
      }

      void UpdateCameraAspect(float aspect)
      {
        orthoProjection = Math::Mat4::orthographic(-GetCamera()->aspect * GetCamera()->orthoScale,
                                                   GetCamera()->aspect * GetCamera()->orthoScale,
                                                   -GetCamera()->orthoScale, GetCamera()->orthoScale,
                                                   -100, 100);
      }

      void UpdateFov(float fov)
      {
        perspProjection = Math::Mat4::perspective(Math::ToRadians(GetCamera()->fov), 1.33333333333, 0.1, 100);
      }

      // Should only be called once!
      void SetProjection(Projection _projection)
      {
        projection = _projection;
      }

      Camera *GetCamera()
      {
        static Camera camera{};
        return &camera;
      }

      void Construct(Data *data, int shaderIdx)
      {
        using namespace Temp::Render;

        data->shaderProgram = OpenGLWrapper::CreateShaderProgram(shaderIdx);

        // Create vertex array object (VAO)
        data->VAO = OpenGLWrapper::CreateVAO();

        // Create vertex buffer object (VBO)
        data->VBO = OpenGLWrapper::CreateVBO(data->vertices.data(), data->vertices.size());

        // Create Element Buffer Object (EBO) and copy index data
        data->EBO = OpenGLWrapper::CreateEBO(data->indices.data(), data->indices.size());

        // Specify vertex attributes
        // position attribute
        OpenGLWrapper::SetVertexAttribArray(0, 3, 3, 0);

        // Unbind VAO and VBO and EBO
        // OpenGLWrapper::UnbindBuffers();
      }

      // TODO: OpenGLWrapper::Set4x4MatrixShaderProperty needs to be set everytime projection changes
      // Might need to set projection and view inside the data
      void Draw(Data *data)
      {
        using namespace Temp::Render;

        // std::cout << "GOT TO DRAW!" << std::endl;

        // data->model = data->model.rotate(0.01f * Math::ToRadians(50.0f), Math::Vec3f(0.5, 1.0, 0));

        OpenGLWrapper::Set4x4MatrixShaderProperty(data->shaderProgram, "model", &data->model.rows[0][0]);
        OpenGLWrapper::Set4x4MatrixShaderProperty(data->shaderProgram, "view", &GetCamera()->view.rows[0][0]);
        OpenGLWrapper::Set4x4MatrixShaderProperty(data->shaderProgram, "projection", &GetProjection().rows[0][0]);

        OpenGLWrapper::BindTexture(GL_TEXTURE0, data->texture);

        // std::cout << data->indices.size() << " " << data->vertices.size() << std::endl;

        // Bind the VAO and draw the triangle
        OpenGLWrapper::DrawElementsInstanced(data->VAO, data->indices.size(), data->numInstances);
      }
    }
  }
}