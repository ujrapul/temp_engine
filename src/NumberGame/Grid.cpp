#include "Grid.hpp"
#include "Components/ComponentType.hpp"

namespace Game::Grid
{
  namespace
  {
    std::vector<float> &Vertices()
    {
      using namespace Temp;

      static std::vector<float> vertices = {
          // positions
          0.5f, 0.5f, 0.0f,   // top right
          0.5f, -0.5f, 0.0f,  // bottom right
          -0.5f, -0.5f, 0.0f, // bottom left
          -0.5f, 0.5f, 0.0f,  // top left
      };

      return vertices;
    }

    std::vector<float> &TexCoords()
    {
      using namespace Temp;

      static std::vector<float> texCoords = {
          // texture coords
          Font::Characters[0].right, 0.0f,                       // top right
          Font::Characters[0].right, Font::Characters[0].bottom, // bottom right
          0.0f, Font::Characters[0].bottom,                      // bottom left
          0.0f, 0.0f                                             // top left
      };

      return texCoords;
    }

    std::vector<GLuint> &Indices()
    {
      using namespace Temp;

      // Define the indices
      static std::vector<GLuint> indices = {
          0, 1, 3, // First Triangle
          1, 2, 3  // Second Triangle
      };

      return indices;
    }

    inline void UpdateUVOffsetsProper(Temp::Scene::Data *data, Data *grid)
    {
      using namespace Temp;
      using namespace Temp::Render;

      // We can't check by entities here because the first entity is non-zero
      for (size_t i = 0; i < grid->gridEntities.size(); ++i)
      {
        auto value = Game::Scene::GetValue(*data, grid->gridEntities[i]);
        grid->gridUVOffsets[i * 2] = Font::Characters[value].left;
        grid->gridUVOffsets[i * 2 + 1] = Font::Characters[value].top;
      }
      OpenGLWrapper::UpdateVBO(grid->uvOffsetVBO, grid->gridUVOffsets.data(), grid->gridUVOffsets.size());
    }

    constexpr void UpdateUVOffsetsNoOp(Temp::Scene::Data * /*data*/, Data * /*grid*/)
    {
    }

    void (*UpdateUVOffsetsFunc)(Temp::Scene::Data *, Data *grid){UpdateUVOffsetsNoOp};
  }

  void Construct(Temp::Scene::Data *data, Data *grid)
  {
    using namespace Temp;
    using namespace Temp::Render;

    grid->entity = Temp::Scene::CreateEntity(*data);
    Temp::Scene::AddComponent<Temp::Component::Type::DRAWABLE>(*data, grid->entity, {});

    auto &drawable = Temp::Scene::Get<Temp::Component::Type::DRAWABLE>(*data, grid->entity);

    drawable.numInstances = grid->gridSize * grid->gridSize;

    srand(static_cast<uint>(time(NULL)));
    for (auto row = 0; row < grid->gridSize; ++row)
    {
      for (auto col = 0; col < grid->gridSize; ++col)
      {
        Temp::Entity entity = Temp::Scene::CreateEntity(*data);
        Temp::Scene::AddComponent<Temp::Component::Type::POSITION2D>(*data, entity, Temp::Math::Vec2f{(float)row, (float)col});
        Temp::Scene::AddComponent<Game::Component::Type::VALUE>(*data, entity, '0' + rand() % 10);
        grid->gridEntities.push_back(entity);
        grid->gridTranslations.push_back(-grid->gridSize / 2 + row);
        grid->gridTranslations.push_back(-grid->gridSize / 2 + col);
      }
    }

  }

  void ConstructRender(Temp::Scene::Data *data, Data *grid)
  {
    using namespace Temp;
    using namespace Temp::Render;

    grid->gridUVOffsets.reserve(grid->gridSize * grid->gridSize);
    for (auto entity : grid->gridEntities)
    {
      auto value = Temp::Scene::Get<Component::Type::VALUE>(*data, entity);
      grid->gridUVOffsets.push_back(Font::Characters[value].left);
      grid->gridUVOffsets.push_back(Font::Characters[value].top);
    }

    auto &drawable = Temp::Scene::Get<Temp::Component::Type::DRAWABLE>(*data, grid->entity);

    drawable.vertices = Vertices();
    drawable.indices = Indices();

    Temp::Component::Drawable::Construct(&drawable, Temp::Render::OpenGLWrapper::ShaderIdx::GRID);

    grid->uvVBO = OpenGLWrapper::CreateVBO(TexCoords().data(), TexCoords().size());
    OpenGLWrapper::SetVertexAttribArray(1, 2, 2, 0);

    grid->translationVBO = OpenGLWrapper::CreateVBO(grid->gridTranslations.data(), sizeof(int), grid->gridTranslations.size());
    OpenGLWrapper::SetVertexAttribIArrayInstanced(2, 2, 2, 0);

    grid->uvOffsetVBO = OpenGLWrapper::CreateVBO(grid->gridUVOffsets.data(), grid->gridUVOffsets.size());
    OpenGLWrapper::SetVertexAttribArrayInstanced(3, 2, 2, 0);

    // Unbind VAO and VBO and EBO
    OpenGLWrapper::UnbindBuffers();

    // load and create a texture
    // -------------------------
    // drawable.texture = OpenGLWrapper::LoadTexture((ApplicationDirectory / "Images" / "awesomeface.png").c_str(), GL_RGBA);
    drawable.texture = Font::Characters[0].texture;

    // Needs to be called to set variables in the shader!
    OpenGLWrapper::Set1IntShaderProperty(drawable.shaderProgram, "texture1", 0);

    UpdateUVOffsetsFunc = UpdateUVOffsetsProper;
  }

  // Add one layer of indirection so that the function isn't called before construction occurs
  void UpdateUVOffsets(Temp::Scene::Data *data, Data *grid)
  {
    UpdateUVOffsetsFunc(data, grid);
  }

  void UpdateNumbers(Temp::Scene::Data *sceneData, Data *grid, Temp::Entity player, int currentValue)
  {
    for (auto entity : grid->gridEntities)
    {
      char &value = Game::Scene::GetValue(*sceneData, entity);
      if (value - '0' == currentValue)
      {
        value = '-';
        Game::Scene::Player::addScore(player, sceneData, 1);
      }
    }
  }
}
