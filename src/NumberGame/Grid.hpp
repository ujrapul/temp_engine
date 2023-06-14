#pragma once

#include "Entity.hpp"
#include "Scene.hpp"
#include "Math.hpp"
#include "RenderUtils.hpp"
#include "ComponentType.hpp"
#include <vector>

namespace Game
{
  namespace Grid
  {
    struct Data
    {
      Temp::Entity entity{};
      std::vector<Temp::Entity> gridEntities{};
      std::vector<int> gridTranslations{};
      std::vector<float> gridUVOffsets{};
      Temp::Render::Square square{};
      int gridSize{10};
      GLuint translationVBO;
      GLuint uvOffsetVBO;
    };

    inline void Construct(Temp::Scene::Data *data, Data *grid)
    {
      using namespace Temp;
      using namespace Temp::Render;

      grid->entity = Temp::Scene::CreateEntity(*data);
      Temp::Scene::AddComponent<Temp::Component::Type::DRAWABLE>(*data, grid->entity, {});

      auto& drawable = Temp::Scene::Get<Temp::Component::Type::DRAWABLE>(*data, grid->entity);

      drawable.vertices.resize(20);
      std::copy(grid->square.fontVertices, grid->square.fontVertices + 20, drawable.vertices.begin());

      drawable.indices.resize(6);
      std::copy(grid->square.indices, grid->square.indices + 6, drawable.indices.begin());

      drawable.numInstances = grid->gridSize * grid->gridSize;

      for (auto row = 0; row < grid->gridSize; ++row)
      {
        for (auto col = 0; col < grid->gridSize; ++col)
        {
          Temp::Entity entity = Temp::Scene::CreateEntity(*data);
          Temp::Scene::AddComponent<Temp::Component::Type::POSITION2D>(*data, entity, Temp::Math::Vec2f{(float)row, (float)col});
          Temp::Scene::AddComponent<Component::Type::VALUE>(*data, entity, '0' + rand() % 10);
          grid->gridEntities.push_back(entity);
          grid->gridTranslations.push_back(-grid->gridSize / 2 + row);
          grid->gridTranslations.push_back(-grid->gridSize / 2 + col);
          grid->gridUVOffsets.push_back(6.f/11.f);
          grid->gridUVOffsets.push_back(6.f/12.f);
        }
      }
    }

    inline void ConstructRender(Temp::Scene::Data *data, Data *grid)
    {
      using namespace Temp;
      using namespace Temp::Render;

      auto& drawable = Temp::Scene::Get<Temp::Component::Type::DRAWABLE>(*data, grid->entity);
      Temp::Component::Drawable::Construct(&drawable, Temp::Render::OpenGLWrapper::ShaderIdx::GRID);

      // Specify vertex attributes
      // position attribute
      OpenGLWrapper::SetVertexAttribArray(0, 3, 5, 0);
      OpenGLWrapper::SetVertexAttribArray(1, 2, 5, 3);

      grid->translationVBO = OpenGLWrapper::CreateVBO(grid->gridTranslations.data(), sizeof(int), grid->gridTranslations.size());
      OpenGLWrapper::SetVertexAttribIArrayInstanced(2, 2, 2, 0);

      grid->uvOffsetVBO = OpenGLWrapper::CreateVBO(grid->gridUVOffsets.data(), sizeof(float), grid->gridUVOffsets.size());
      OpenGLWrapper::SetVertexAttribArrayInstanced(3, 2, 2, 0);

      // Unbind VAO and VBO and EBO
      OpenGLWrapper::UnbindBuffers();

      // load and create a texture
      // -------------------------
      // drawable.texture = OpenGLWrapper::LoadTexture((ApplicationDirectory / "Images" / "awesomeface.png").c_str(), GL_RGBA);
      drawable.texture = Font::Characters['A'].TextureID;

      // Needs to be called to set variables in the shader!
      OpenGLWrapper::Set1IntShaderProperty(drawable.shaderProgram, "texture1", 0);
    }

    // Should be queued into the RenderThread
    inline void ConstructRenderVoid(Temp::Scene::Data *data, void *grid)
    {
      ConstructRender(data, static_cast<Data*>(grid));
    }

  }
}
